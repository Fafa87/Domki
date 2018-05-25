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
void konfiguruj(int l)
{
	if (l == 0)
	{
		wykonaj("serwer");
		wykonaj("start");

	}
	else
	{ 
		wykonaj("klient " + std::to_string(l));
		wykonaj("polacz localhost:85");
		wykonaj("gotowy");
	}
}

void wykonaj(string zadanie)
{
	if (zadanie == "serwer")
	{
		serwer = new Serwer();
		auto adres = serwer->Postaw();
		printf("%s:%d\n", adres.ip.c_str(), adres.port);

		serwer->OczekujNaGracza();
		serwer->OczekujNaGracza();
	}
	if (zadanie.find("klient") == 0)
	{
		klient = new Klient(zadanie.substr(7));
		printf("Klient: %s\n", klient->nazwa.c_str());
	}
	if (zadanie.find("polacz") == 0)
	{
		auto cel = zadanie.substr(7);
		auto ip_port = split(cel, ':');
		Adres adres(ip_port[0], stoi(ip_port[1]));
		klient->Podlacz(adres);
	}
	if (zadanie.find("start") == 0)
	{
		MisjaUstawienia ustawienia;
		ustawienia.nazwa = "graf_testowy.txt";
		ustawienia.komputery.clear();
		ustawienia.nr_gracza = 0;

		serwer->Start(ustawienia);

		//serwer->ludzie[0].wtyk->setBlocking(false);
		//serwer->ludzie[1].wtyk->setBlocking(false);
		
		SerwerowyRuszacz ruszacz(*serwer);

		misja(ustawienia, ruszacz);

		string test = "Rozgrywka:A";
		/*while (1)
		{
			auto& res = serwer->Odbierz();
			for (auto s : res)
				printf("odebralem: %s\n", s.c_str());

			Sleep(400);
			serwer->Rozeslij(test);
			test[10] = (test[10] + 1) % 150;
		}*/
	}
	if (zadanie.find("gotowy") == 0)
	{
		auto res = klient->OczekujNaStart();
		printf("startuje misje %s\n", res.second.nazwa.c_str());

		string test = "A";
		klient->wtyk->setBlocking(false);

		KlientowyRuszacz ruszacz(*klient);

		res.second.komputery.clear();
		misja(res.second, ruszacz);

		/*while (1)
		{
			auto& res = klient->Odbierz();
			if (res.first)
			{
				printf("sync rozgr: %s\n", res.second.c_str());
			}

			Sleep(600);
			klient->Wyslij(test);
			test[0] = (test[0] + 1) % 150;
		}*/
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
		konfiguruj(int(argv[1][0] - '0'));

	do 
	{
		char tmp[1000];
		gets_s(tmp);
		zadanie = tmp;

		wykonaj(zadanie);

		Sleep(10);
	} while (zadanie != "k");

	return 0;
}
