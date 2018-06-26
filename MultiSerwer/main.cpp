#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include <SFML/Graphics.hpp>

#include "../Domki/misja.h"
#include "../Domki/os.h"

#include "../Domki/multi.h"
#include "../Domki/multi_dzialacze.h"

using namespace std;
using namespace multi;

Serwer* serwer = NULL;
Klient* klient = NULL;
MisjaUstawienia misja_ustawienia;

#include <iterator>

template<typename Out>
void split(const std::string &s, char delim, Out result) {
	std::stringstream ss;
	ss.str(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		*(result++) = item;
	}
}

std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, std::back_inserter(elems));
	return elems;
}

void wykonaj(string zadanie);
void konfiguruj(int l, const char * argv[])
{
	if (l == 0)
	{
		wykonaj("serwer " + string(argv[2]));
		wykonaj("start");
	}
	else
	{ 
		wykonaj("klient " + std::to_string(l));
		wykonaj("polacz localhost:85");
		wykonaj("gotowy");
	}
}

void komunikat()
{
	if (serwer == nullptr && klient == nullptr)
	{
		printf("Zdecyduj czy jestes serwerem czy graczem:\n- jesli serwer to wpisz: 'serwer <nazwa_planszy>.txt' lub 'serwer <nr_planszy>'\n- jesli graczem to wpisz: 'klient <nazwa>'\n");
	}
	else if (klient != nullptr)
	{
		printf("Jestes graczem, musisz sie polaczyc z serwerm. Wpisz: 'polacz' lub 'polacz <adres_ip_serwera>'\n");
	}
	else if (serwer != nullptr)
	{
		printf("Jestes serwerem, gracze sie podlaczyli, napisz 'start' aby zaczac gre:\n");
	}
}

void wykonaj(string zadanie)
{
	if (serwer == nullptr && klient == nullptr)
	{
		if (zadanie.find("serwer") == 0)
		{
			serwer = new Serwer();
			auto adres = serwer->Postaw();
			printf("%s\n", adres.ToString().c_str());

			auto misja_nazwa = zadanie.substr(7);
			if (misja_nazwa.size() == 1)
			{
				misja_nazwa = wczytaj_liste_plansz()[atoi(misja_nazwa.c_str())];
			}
			auto misja_sciezka = "Plansza\\" + misja_nazwa;

			misja_ustawienia = wczytaj_meta(misja_sciezka);
			misja_ustawienia.nazwa = misja_nazwa;
			int liczba_graczy = misja_ustawienia.komputery.size() + 1;

			printf("Stworzona gra na planszy %s\n", misja_nazwa.c_str());
			printf("Oczekuj na podlaczenie %d graczy\n", liczba_graczy);

			for (int i = 0; i<liczba_graczy; i++)
				serwer->OczekujNaGracza();
		}
		if (zadanie.find("klient") == 0)
		{
			klient = new Klient(zadanie.substr(7));
			printf("Klient: %s\n", klient->nazwa.c_str());
		}
	}
	else if (klient != nullptr)
	{
		if (zadanie.find("polacz") == 0)
		{
			Adres adres;
			if (klient->lista_serwerow.size() == 0)
			{
				auto cel = zadanie.substr(7);
				auto ip_port = split(cel, ':');
				adres = Adres(ip_port[0], stoi(ip_port[1]));
			}
			else
			{
				adres = klient->lista_serwerow.back();
			}
			klient->Podlacz(adres);

			// nie ma co czekac na gotowy
			//if (zadanie.find("gotowy") == 0)
			{
				auto res = klient->OczekujNaStart();
				printf("startuje misje %s\n", res.second.nazwa.c_str());

				string test = "A";
				klient->wtyk->setBlocking(false);

				KlientowyRuszacz ruszacz(*klient);

				res.second.komputery.clear();
				misja(res.second, ruszacz);
				
				klient->wtyk->setBlocking(true);
			}
		}
	}
	else if (serwer != nullptr)
	{
		if (zadanie.find("start") == 0)
		{
			MisjaUstawienia ustawienia = misja_ustawienia;
			ustawienia.komputery.clear();
			ustawienia.nr_gracza = 0;

			serwer->Start(ustawienia);

			//serwer->ludzie[0].wtyk->setBlocking(false);
			//serwer->ludzie[1].wtyk->setBlocking(false);

			SerwerowyRuszacz ruszacz(*serwer);

			misja(ustawienia, ruszacz);

			string test = "Rozgrywka:A";
			
			delete serwer;
			serwer = nullptr;
		}
	}
	
	if (zadanie.find("odbierz") == 0)
	{
		vector<vector<string>> wiad;
		/*if (serwer != NULL)
		wiad = serwer->Odbierz();*/
		if (klient != NULL)
			wiad.push_back(Pobierz(*klient->wtyk));
		printf("Odebralem:\n");
		for (auto l : wiad)
		{
			for (auto s : l)
				printf("'%s'\n", s.c_str());
			printf("----\n");
		}
	}
	if (zadanie.find("napisz") == 0)
	{
		auto text = zadanie.substr(7);
		/*if (serwer != NULL)
			serwer->Rozeslij(text);
		if (klient != NULL)
			Wyslij(*klient->wtyk, text);*/
	}
}

int main(int argc, const char * argv[]) {
	string zadanie;

	if (argc > 1)
		konfiguruj(int(argv[1][0] - '0'), argv);

	do 
	{
		komunikat();
		char tmp[1000];
		gets_s(tmp);
		zadanie = tmp;

		wykonaj(zadanie);

		if (klient != nullptr)
		{			
			klient->lista_serwerow.clear();
			klient->SpiszSerwery();
		}

		Sleep(10);
	} while (zadanie != "k");

	return 0;
}
