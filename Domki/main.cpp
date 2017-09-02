#include <SFML/Graphics.hpp>

#include "dane.h"
#include "dzialacze.h"
#include<windows.h>

Rozgrywka prosta_rozgrywka()
{
	Rozgrywka gra;
	// wa¿ne aby vector nie zmienia³ po³o¿enia obiektów
	gra.gracze.reserve(100); 	
	gra.domki.reserve(100);
	gra.armie.reserve(100);

	// gracze
	gra.gracze.push_back(Gracz());
	Gracz& gracz1 = gra.gracze[0];
	gracz1.numer = 1; gracz1.nazwa = "Stasio";
	gracz1.kolor = sf::Color::Green;

	gra.gracze.push_back(Gracz());
	Gracz& gracz2 = gra.gracze[1];
	gracz2.numer = 2; gracz2.nazwa = "Tomek";
	gracz2.kolor = sf::Color::Red;

	gra.gracze.push_back(Gracz());
	Gracz& gracz3 = gra.gracze[2];
	gracz3.numer = 0; gracz2.nazwa = "Nikt";
	gracz3.kolor = sf::Color::Color(80, 80, 80);

	// domki
	gra.domki.push_back(Domek());
	Domek& domek1 = gra.domki.back();
	domek1.gracz = &gracz1;
	domek1.polozenie = { 100.0, 100.0 };
	domek1.produkcja = 0.2;
	domek1.max_liczebnosc = 100;
	domek1.wyglad = Wyglad::kDomek;
	gra.ZmienLiczebnosc(domek1, 40);
	
	gra.domki.push_back(Domek());
	Domek& domek2 = gra.domki.back();
	domek2.gracz = &gracz2;
	domek2.polozenie = { 500.0, 300.0 };
	domek2.produkcja = 0.5;
	domek2.max_liczebnosc = 30;
	domek2.wyglad = Wyglad::kDomek;
	gra.ZmienLiczebnosc(domek2, 20);

	gra.domki.push_back(Domek());
	Domek& domek3 = gra.domki.back();
	domek3.gracz = &gracz2;
	domek3.polozenie = { 530.0, 400.0 };
	domek3.produkcja = 0.5;
	domek3.max_liczebnosc = 30;
	domek3.wyglad = Wyglad::kDomek;
	gra.ZmienLiczebnosc(domek3, 20);

	gra.domki.push_back(Domek());
	Domek& domek4 = gra.domki.back();
	domek4.gracz = &gracz3;
	domek4.polozenie = { 290.0, 600.0 };
	domek4.produkcja = 0.2;
	domek4.max_liczebnosc = 100;
	domek4.wyglad = Wyglad::kDomek;
	gra.ZmienLiczebnosc(domek4, 30);

	// armie
	gra.armie.push_back(Ludek(domek2));
	Ludek& armia1 = gra.armie.back();
	armia1.gracz = &gracz1;
	armia1.polozenie = { 150.0, 110.0 };
	armia1.wyglad = Wyglad::kLudek;
	gra.ZmienLiczebnosc(armia1, 6);

	gra.armie.push_back(Ludek(domek1));
	Ludek& armia2 = gra.armie.back();
	armia2.gracz = &gracz2;
	armia2.polozenie = { 200.0, 400.0 };
	armia2.wyglad = Wyglad::kLudek;
	gra.ZmienLiczebnosc(armia2, 30);

	return gra;
}

int main()
{
	auto videoMode = sf::VideoMode(1600, 800);
	sf::RenderWindow window(videoMode, "DOMKI PRE-ALFA!", sf::Style::Fullscreen);

	// ustawiamy t³o
	sf::Texture backtexture;
	backtexture.loadFromFile("Grafika\\kafle.jpg");
	backtexture.setRepeated(true);
	sf::Sprite background(backtexture);
	background.setTextureRect({ 0, 0, (int)window.getSize().x, (int)window.getSize().y });

	// fpsy
	sf::Font czcionka;
	czcionka.loadFromFile("Grafika\\waltographUI.ttf");

	sf::Text podpis;
	podpis.setFont(czcionka);
	podpis.setCharacterSize(50);
	podpis.setStyle(sf::Text::Bold);
	podpis.setFillColor(sf::Color::Green);
	podpis.move(300,0);
	
	// tworzymy rozgrywke
	Rozgrywka rozgrywka = prosta_rozgrywka();

	// przygotowujemy dzialaczy
	Wyswietlacz wyswietlacz(rozgrywka);
	MyszDecydent myszkaGracza(rozgrywka, rozgrywka.gracze[0]);
	MyszDecydent myszkaGracza2(rozgrywka, rozgrywka.gracze[1]);
	Ruszacz ruszacz(rozgrywka);
	clock_t czasomierz;
	czasomierz = clock();
	double czasik = 0;
	while (window.isOpen())
	{
		sf::Event event;

		myszkaGracza.Przetworz(); // puste
		myszkaGracza2.Przetworz(); // puste
		while (window.pollEvent(event))
		{
			myszkaGracza.Przetworz(event); // pewnie tutaj mo¿na z³apaæ, ¿e naciœniêta zosta³a myszka
			myszkaGracza2.Przetworz(event); // pewnie tutaj mo¿na z³apaæ, ¿e naciœniêta zosta³a myszka
			switch (event.type)
			{
			case sf::Event::KeyReleased:
				switch (event.key.code)
				{
				case sf::Keyboard::Escape:
					return 0;
				}
				break;
			}
			if (event.type == sf::Event::Closed)
				window.close();
		}

		myszkaGracza.WykonajRuch();
		myszkaGracza2.WykonajRuch();
		ruszacz.Ruszaj();

		czasomierz = clock() - czasomierz;
		czasik = (double)czasomierz / CLOCKS_PER_SEC;
		podpis.setString("FPSY: "+std::to_string((int)(1.0/czasik+0.5)));
		czasomierz = clock();

		window.clear();
		window.draw(background);
		window.draw(podpis);//FPSY

		wyswietlacz.Wyswietlaj(window);
		window.display();
		Sleep(500);
		
	}
	return 0;
}