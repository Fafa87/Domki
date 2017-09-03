#include <SFML/Graphics.hpp>

#include "dane.h"
#include "dzialacze.h"
#include<windows.h>
#include<fstream>
Rozgrywka zwarcie_rozgrywka()
{
	Rozgrywka gra;
	//gracze
	gra.gracze.push_back(Gracz());
	Gracz& gracz1 = gra.gracze.front();
	gracz1.numer = 1; gracz1.nazwa = "GRACZ";
	gracz1.kolor = sf::Color::Red;

	gra.gracze.push_back(Gracz());
	Gracz& gracz2 = *(++gra.gracze.begin());
	gracz2.numer = 2; gracz2.nazwa = "KOMPUTER";
	gracz2.kolor = sf::Color::Blue;

	gra.gracze.push_back(Gracz());
	Gracz& gracz3 = *(++(++gra.gracze.begin()));
	gracz3.numer = 3; gracz3.nazwa = "GRA";
	gracz3.kolor = sf::Color::White;
	gracz3.aktywny = false;
	//domki
	ifstream plikmapa;
	plikmapa.open("Plansza\\tomek_test.txt");
	char znak;
	for (int a = 0; a < 4; a++)
		for (int b = 0; b < 7; b++)
			{
			plikmapa >> znak;
			if (znak != '.')
				{
				gra.domki.push_back(Domek());
				Domek& domek = gra.domki.back();
				if(znak=='1')domek.gracz = &gracz1;
				else if (znak == '2')domek.gracz = &gracz2;
				else if (znak == '3')domek.gracz = &gracz3;
				domek.polozenie = { (float)(200+b*100),(float)(300+a*100) };
				domek.produkcja = 2;
				domek.max_liczebnosc = 100;
				domek.wyglad = Wyglad::kDomek;
				if (znak == '3')gra.ZmienLiczebnosc(domek, 25);
				else gra.ZmienLiczebnosc(domek, 50);
				}
			}
	plikmapa.close();
	return gra;
}
Rozgrywka prosta_rozgrywka()
{
	Rozgrywka gra;
	// wa�ne aby vector nie zmienia� po�o�enia obiekt�w
	//gra.gracze.reserve(100); 	
	//gra.domki.reserve(100);
	//gra.armie.reserve(100);

	// gracze
	gra.gracze.push_back(Gracz());
	Gracz& gracz1 = gra.gracze.back();
	gracz1.numer = 1; gracz1.nazwa = "Stasio";
	gracz1.kolor = sf::Color::Green;

	gra.gracze.push_back(Gracz());
	Gracz& gracz2 = gra.gracze.back();
	gracz2.numer = 2; gracz2.nazwa = "Tomek";
	gracz2.kolor = sf::Color::Red;

	gra.gracze.push_back(Gracz());
	Gracz& gracz3 = gra.gracze.back();
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
	Domek& domek1b = gra.domki.back();
	domek1b.gracz = &gracz1;
	domek1b.polozenie = { 100.0, 200.0 };
	domek1b.produkcja = 2;
	domek1b.max_liczebnosc = 100;
	domek1b.wyglad = Wyglad::kDomek;
	gra.ZmienLiczebnosc(domek1b, 20);
	
	gra.domki.push_back(Domek());
	Domek& domek2 = gra.domki.back();
	domek2.gracz = &gracz2;
	domek2.polozenie = { 500.0, 300.0 };
	domek2.produkcja = 5;
	domek2.max_liczebnosc = 30;
	domek2.wyglad = Wyglad::kDomek;
	gra.ZmienLiczebnosc(domek2, 20);

	gra.domki.push_back(Domek());
	Domek& domek3 = gra.domki.back();
	domek3.gracz = &gracz2;
	domek3.polozenie = { 530.0, 400.0 };
	domek3.produkcja = 5;
	domek3.max_liczebnosc = 30;
	domek3.wyglad = Wyglad::kDomek;
	gra.ZmienLiczebnosc(domek3, 20);

	gra.domki.push_back(Domek());
	Domek& domek4 = gra.domki.back();
	domek4.gracz = &gracz3;
	domek4.polozenie = { 290.0, 600.0 };
	domek4.produkcja = 2;
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

	// ustawiamy t�o
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
	//Rozgrywka rozgrywka = prosta_rozgrywka();
	Rozgrywka rozgrywka = zwarcie_rozgrywka();
	// przygotowujemy dzialaczy
	Wyswietlacz wyswietlacz(rozgrywka);
	MyszDecydent myszkaGracza(rozgrywka, rozgrywka.gracze.front());
	MyszDecydent myszkaGracza2(rozgrywka, rozgrywka.gracze.back());
	Komputer kompiuter(rozgrywka, (*(++rozgrywka.gracze.begin())));
	
	Ruszacz ruszacz(rozgrywka);
	//czasomierz
	clock_t czasomierz;
	czasomierz = clock();
	int czasik = 0;
	float czas;
	//APM
	long long akcje = 0;
	float czas_przeminal = clock();
	while (window.isOpen())
	{
		sf::Event event;

		myszkaGracza.Przetworz(); // puste
		myszkaGracza2.Przetworz(); // puste
		while (window.pollEvent(event))
		{
			myszkaGracza.Przetworz(event); // pewnie tutaj mo�na z�apa�, �e naci�ni�ta zosta�a myszka
			myszkaGracza2.Przetworz(event); // pewnie tutaj mo�na z�apa�, �e naci�ni�ta zosta�1a myszka
			if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::KeyReleased)akcje++;
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

		
		
		
		///FPSY
		czas = (double)(clock() - czasomierz) / CLOCKS_PER_SEC;
		myszkaGracza.WykonajRuch();
		//myszkaGracza2.WykonajRuch();
		kompiuter.czas += czas;
		kompiuter.WykonajRuch();
		ruszacz.Ruszaj(czas);

		window.clear();
		window.draw(background);

		///FPSY
		
		czasik = (int)(1.0 / czas + 0.5);
		podpis.setString("FPSY: " + std::to_string(czasik));
		podpis.setPosition(300, 0);
		window.draw(podpis);//FPSY

		///APM
		podpis.setPosition(700, 0);
		czas_przeminal = (double)(clock() - czas_przeminal) / CLOCKS_PER_SEC;
		podpis.setString("APM: " + std::to_string((int)(60 * akcje / czas_przeminal)));
		window.draw(podpis); //APM

		wyswietlacz.Wyswietlaj(window);
		window.display();

		czasomierz = clock();

		Sleep(16);
	}
	return 0;
}