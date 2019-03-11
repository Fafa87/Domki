#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include <SFML/Graphics.hpp>

#include "../Domki/misja.h"
#include "../Domki/os.h"

using namespace std;

#include <iterator>

void dodaj_gracza(Rozgrywka & gra, Gracz& g, double x, double y)
{
	gra.domki.push_back(Domek());
	Domek& domek = gra.domki.back();
	domek.polozenie = { x,y };
	domek.produkcja = 2;
	domek.max_liczebnosc = 100;
	domek.wyglad = Wyglad::kMiasto;
	domek.gracz = &g;
	domek.poziom = 1;
	gra.ZmienLiczebnosc(domek, 50);

	gra.domki.push_back(Domek());
	Domek& domek2 = gra.domki.back();
	domek2.polozenie = { x,y + 150};
	domek2.produkcja = 2;
	domek2.max_liczebnosc = 100;
	domek2.wyglad = Wyglad::kMiasto;
	domek2.gracz = &g;
	domek2.poziom = 10;
	gra.ZmienLiczebnosc(domek2, 250);
	domek2.drogi.push_back(&domek);

	gra.domki.push_back(Domek());
	Domek& domek3 = gra.domki.back();
	domek3.polozenie = { x - 100 ,y + 150 };
	domek3.typdomku = TypDomku::kZbrojownia;
	domek3.gracz = &g;
	domek3.poziom = 1;
	gra.ZmienLiczebnosc(domek3, 50);
	domek3.drogi.push_back(&domek);

	gra.domki.push_back(Domek());
	Domek& domek4 = gra.domki.back();
	domek4.polozenie = { x + 100 ,y + 150 };
	domek4.typdomku = TypDomku::kFort;
	domek4.gracz = &g;
	domek4.poziom = 1;
	gra.ZmienLiczebnosc(domek4, 50);
	domek4.drogi.push_back(&domek);

	gra.domki.push_back(Domek());
	Domek& domek5 = gra.domki.back();
	domek5.polozenie = { x + 100 ,y + 250 };
	domek5.typdomku = TypDomku::kMiasto;
	domek5.gracz = &g;
	domek5.poziom = 0;
	gra.ZmienLiczebnosc(domek5, 50);
	domek5.drogi.push_back(&domek);
	domek5.drogi.push_back(&domek4);

	gra.armie.push_back(Ludek(domek));
	Ludek & ludek = gra.armie.back();
	ludek.polozenie = { x + 100,y };
	ludek.gracz = &g;
	ludek.wyglad = Wyglad::kWojownik;
	ludek.tarcza = 23;
	gra.ZmienLiczebnosc(ludek, 40);

	gra.armie.push_back(Ludek(domek));
	Ludek & ludek2 = gra.armie.back();
	ludek2.polozenie = { x - 100,y };
	ludek2.gracz = &g;
	ludek2.wyglad = Wyglad::kWojownik;
	gra.ZmienLiczebnosc(ludek2, 240);
}


Rozgrywka pokazowa_rozgrywka()
{
	Rozgrywka gra;
	//gracze
	gra.gracze.push_back(Gracz());
	Gracz& gracz0 = gra.gracze.back();
	gracz0.numer = 0; gracz0.nazwa = "NEUTRAL";
	gracz0.kolor = sf::Color(128,128,128);
	gracz0.aktywny = false;

	gra.gracze.push_back(Gracz());
	Gracz& gracz1 = gra.gracze.back();
	gracz1.numer = 1; gracz1.nazwa = "GRACZ1";
	gracz1.kolor = sf::Color::Red;

	gra.gracze.push_back(Gracz());
	Gracz& gracz2 = gra.gracze.back();
	gracz2.numer = 2; gracz2.nazwa = "KOMPUTER2";
	gracz2.kolor = sf::Color::Blue;

	gra.gracze.push_back(Gracz());
	Gracz& gracz3 = gra.gracze.back();
	gracz3.numer = 3; gracz3.nazwa = "KOMPUTER3";
	gracz3.kolor = sf::Color::Green;

	gra.gracze.push_back(Gracz());
	Gracz& gracz4 = gra.gracze.back();
	gracz4.numer = 4; gracz4.nazwa = "KOMPUTER4";
	gracz4.kolor = sf::Color::Yellow;
	
	dodaj_gracza(gra, gracz0, 200, 200);
	dodaj_gracza(gra, gracz1, 500, 200);
	dodaj_gracza(gra, gracz2, 500, 600);
	dodaj_gracza(gra, gracz3, 800, 200);
	dodaj_gracza(gra, gracz4, 800, 600);

	return gra;
}

int pokazowa_misja()
{
	sf::ContextSettings ustawienia;
	ustawienia.antialiasingLevel = 8;

	auto videoMode = sf::VideoMode(1600, 899);
	sf::RenderWindow window(videoMode, "DOMKI PRE-ALFA!", sf::Style::None, ustawienia);

	// tworzymy rozgrywke
	Rozgrywka rozgrywka = pokazowa_rozgrywka();
	// przygotowujemy dzialaczy
	Wyswietlacz wyswietlacz(rozgrywka);
	wyswietlacz.Zaladuj("wroclaw");
	MyszDecydent myszkaGracza(window, rozgrywka, rozgrywka.Gracz(1));
	OznaczaczWyborow ruchGracza(myszkaGracza);

	sf::View view = sf::View(sf::FloatRect(0, 0, 1600, 900));
	window.setView(view);

	Ruszacz ruszacz;
	ruszacz.rozgrywka = &rozgrywka;
	ruszacz.szybkosc *= 0.0001;

	//czasomierz
	auto czasomierz = clock();
	int czasik = 0;
	//APM
	long long akcje = 0;
	double czas_przeminal = clock();
	while (window.isOpen())
	{
		sf::Event event;

		myszkaGracza.Przetworz(); // puste
		while (window.pollEvent(event))
		{
			myszkaGracza.Przetworz(event);
			if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::KeyReleased)akcje++;
			switch (event.type)
			{
			case sf::Event::KeyReleased:
				switch (event.key.code)
				{
				case sf::Keyboard::Escape:
					return 0;
				case sf::Keyboard::F3:
					wyswietlacz.ZaladujInne();
					break;
				}
				break;
			}
			if (event.type == sf::Event::Closed)
				window.close();
		}

		ruszacz.Ruszaj(0.0001);

		window.clear();
		wyswietlacz.WyswietlTlo(window);

		ruchGracza.Wyswietlaj(window);
		wyswietlacz.Wyswietlaj(window);
		
		window.display();

		Sleep(16);
	}
	return 0;
}

int main(int argc, const char * argv[]) {

	pokazowa_misja();

	return 0;
}
