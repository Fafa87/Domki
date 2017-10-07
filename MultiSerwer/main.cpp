#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include <SFML/Graphics.hpp>

#include "../Domki/misja.h"
#include "../Domki/os.h"

#include "../Domki/serwer.h"

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

int main() {
	string zadanie;
	do 
	{
		char tmp[1000];
		gets_s(tmp);
		zadanie = tmp;

		if (zadanie == "serwer")
		{
			serwer = new Serwer();
			auto adres = serwer->Przygotuj();
			printf("%s:%d\n", adres.ip.c_str(), adres.port);

			serwer->OczekujNaGraczy(2);
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
		if (zadanie.find("odbierz") == 0)
		{
			vector<vector<string>> wiad;
			if (serwer != NULL)
				wiad = serwer->Odbierz();
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
			if (serwer != NULL)
				serwer->Rozeslij(text);
			if (klient != NULL)
				Wyslij(*klient->wtyk, text);
		}


		Sleep(10);
	} while (zadanie != "k");



	return 0;
}
