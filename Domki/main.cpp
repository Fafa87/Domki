#include <SFML/Graphics.hpp>

#include "dane.h"
#include "dzialacze.h"

Rozgrywka prosta_rozgrywka()
{
	Rozgrywka gra;
	// gracze
	gra.gracze.push_back(Gracz());
	Gracz& gracz1 = gra.gracze.back();
	gracz1.numer = 1; gracz1.nazwa = "Stasio";

	gra.gracze.push_back(Gracz());
	Gracz& gracz2 = gra.gracze.back();
	gracz2.numer = 2; gracz2.nazwa = "Tomek";

	// domki
	gra.domki.push_back(Domek());
	Domek& domek1 = gra.domki.back();
	domek1.gracz = &gracz1;
	domek1.polozenie = { 100.0, 100.0 };
	domek1.liczebnosc = 4;
	domek1.produkcja = 0.2;
	domek1.max_liczebnosc = 100;
	domek1.rozmiar = 20;
	domek1.wyglad = Wyglad::kDomek;
	
	gra.domki.push_back(Domek());
	Domek& domek2 = gra.domki.back();
	domek2.gracz = &gracz2;
	domek2.polozenie = { 500.0, 300.0 };
	domek2.liczebnosc = 10;
	domek2.produkcja = 0.5;
	domek2.max_liczebnosc = 30;
	domek2.rozmiar = 10;
	domek2.wyglad = Wyglad::kDomek;

	// armie
	gra.armie.push_back(Ludek(domek2));
	Ludek& armia1 = gra.armie.back();
	armia1.gracz = &gracz1;
	armia1.polozenie = { 150.0, 110.0 };
	armia1.liczebnosc = 5;
	domek1.rozmiar = 5;
	domek1.wyglad = Wyglad::kLudek;

	gra.armie.push_back(Ludek(domek1));
	Ludek& armia2 = gra.armie.back();
	armia2.gracz = &gracz2;
	armia2.polozenie = { 800.0, 400.0 };
	armia2.liczebnosc = 35;
	domek2.rozmiar = 35;
	domek2.wyglad = Wyglad::kLudek;

	return gra;
}

int main()
{
	auto videoMode = sf::VideoMode(1600, 800);
	sf::RenderWindow window(videoMode, "DOMKI PRE-ALFA!", sf::Style::Default);

	// ustawiamy t�o
	sf::Texture backtexture;
	backtexture.loadFromFile("Grafika\\kafle.jpg");
	backtexture.setRepeated(true);
	sf::Sprite background(backtexture);
	background.setTextureRect({ 0, 0, (int)window.getSize().x, (int)window.getSize().y });
	
	// tworzymy rozgrywk�
	Rozgrywka rozgrywka = prosta_rozgrywka();

	// przygotowujemy dzialaczy
	Wyswietlacz wyswietlacz(rozgrywka);
	MyszDecydent myszkaGracza(rozgrywka, rozgrywka.gracze[0]);
	Ruszacz ruszacz(rozgrywka);
	
	while (window.isOpen())
	{
		sf::Event event;

		myszkaGracza.Przetworz(); // puste
		while (window.pollEvent(event))
		{
			myszkaGracza.Przetworz(event); // pewnie tutaj mo�na z�apa�, �e naci�ni�ta zosta�a myszka
			switch (event.type)
			{
			case sf::Event::KeyReleased:
				switch (event.key.code)
				{
				case sf::Keyboard::Escape:
					window.create(videoMode, "Hej!", sf::Style::Default);
					background.setTextureRect({ 0, 0, (int)window.getSize().x, (int)window.getSize().y });
					break;
				case sf::Keyboard::Return:
					window.create(videoMode, "Ho!", sf::Style::Fullscreen);
					background.setTextureRect({ 0, 0, (int)window.getSize().x, (int)window.getSize().y });
					break;
				}
				break;
			}
			if (event.type == sf::Event::Closed)
				window.close();
		}

		myszkaGracza.WykonajRuch();
		ruszacz.Ruszaj();

		window.clear();
		window.draw(background);
		wyswietlacz.Wyswietlaj(window);
		window.display();
	}

	return 0;
}