#include "misja.h"

Rozgrywka zwarcie_rozgrywka(string sciezka)
{
	Rozgrywka gra;
	//gracze
	gra.gracze.push_back(Gracz());
	Gracz& gracz1 = gra.gracze.back();
	gracz1.numer = 1; gracz1.nazwa = "GRACZ";
	gracz1.kolor = sf::Color::Yellow;

	gra.gracze.push_back(Gracz());
	Gracz& gracz2 = gra.gracze.back();
	gracz2.numer = 2; gracz2.nazwa = "KOMPUTER1";
	gracz2.kolor = sf::Color::Red;

	gra.gracze.push_back(Gracz());
	Gracz& gracz3 = gra.gracze.back();
	gracz3.numer = 3; gracz3.nazwa = "KOMPUTER2";
	gracz3.kolor = sf::Color::Blue;

	gra.gracze.push_back(Gracz());
	Gracz& gracz4 = gra.gracze.back();
	gracz4.numer = 4; gracz4.nazwa = "KOMPUTER3";
	gracz4.kolor = sf::Color::Green;

	gra.gracze.push_back(Gracz());
	Gracz& gracz5 = gra.gracze.back();
	gracz5.numer = 5; gracz5.nazwa = "GRA";
	gracz5.kolor = sf::Color::White;
	gracz5.aktywny = false;
	gra.liczba_aktywnych_graczy = 4;
	//domki
	ifstream plikmapa;
	plikmapa.open(sciezka);
	char znak;
	for (int a = 0; a < 4; a++)
		for (int b = 0; b < 9; b++)
		{
			plikmapa >> znak;
			if (znak != '.')
			{
				gra.domki.push_back(Domek());
				Domek& domek = gra.domki.back();
				if (znak == '1')domek.gracz = &gracz1;
				else if (znak == '2')domek.gracz = &gracz2;
				else if (znak == '3')domek.gracz = &gracz3;
				else if (znak == '4')domek.gracz = &gracz4;
				else if (znak == '5')domek.gracz = &gracz5;
				if (znak == '1')gracz1.liczba_tworow++;
				else if (znak == '2')gracz2.liczba_tworow++;
				else if (znak == '3')gracz3.liczba_tworow++;
				else if (znak == '4')gracz4.liczba_tworow++;
				else if (znak == '5')gracz5.liczba_tworow++;
				domek.polozenie = { (float)(200 + b * 150),(float)(300 + a * 150) };
				domek.produkcja = 2;
				domek.max_liczebnosc = 100;
				domek.wyglad = Wyglad::kDomek;
				if (znak == '5')gra.ZmienLiczebnosc(domek, 25);
				else gra.ZmienLiczebnosc(domek, 50);
			}
		}
	if (gracz1.liczba_tworow == 0)
	{
		gracz1.aktywny = false;
		gra.liczba_aktywnych_graczy--;
	}
	if (gracz2.liczba_tworow == 0)
	{
		gracz2.aktywny = false;
		gra.liczba_aktywnych_graczy--;
	}
	if (gracz3.liczba_tworow == 0)
	{
		gracz3.aktywny = false;
		gra.liczba_aktywnych_graczy--;
	}
	if (gracz4.liczba_tworow == 0)
	{
		gracz4.aktywny = false;
		gra.liczba_aktywnych_graczy--;
	}
	plikmapa.close();
	return gra;
}

void odliczanie(sf::Text& podpis, sf::RenderWindow& window, Wyswietlacz& wyswietlacz)
{
	//Przygotuj sie
	podpis.setCharacterSize(250);
	for (int a = 3; a >= 0; a--)
	{
		window.clear();
		wyswietlacz.WyswietlTlo(window);
		if (a>0)podpis.setString(std::to_string(a));
		else podpis.setString("RUSZAJ!");
		if (a>0)podpis.setPosition(700, 200);
		else podpis.setPosition(200, 200);
		wyswietlacz.Wyswietlaj(window);
		window.draw(podpis);
		window.display();
		if (a > 0)Sleep(1000);
		else Sleep(250);
	}
	podpis.setCharacterSize(50);

}


int misja(MisjaUstawienia misja_ustawienia, Ruszacz& ruszacz)
{
	string sciezka = "Plansza\\" + misja_ustawienia.nazwa;
	string trudnosc = misja_ustawienia.trudnosc;
	double predkosc = misja_ustawienia.szybkosc;
	int nr_gracza = misja_ustawienia.nr_gracza;

	sf::ContextSettings ustawienia;
	ustawienia.antialiasingLevel = 8;

	auto videoMode = sf::VideoMode(800, 449); // 1600, 899);
	sf::RenderWindow window(videoMode, "DOMKI PRE-ALFA!", sf::Style::Fullscreen, ustawienia);
	sf::View view(sf::FloatRect(0, 0, 1600, 900)); // 1600, 900));
	window.setView(view);

	if (misja_ustawienia.nr_gracza == 4)
		window.setVisible(false);

	// fpsy
	sf::Font czcionka;
	czcionka.loadFromFile("Grafika\\waltographUI.ttf");

	sf::Text podpis;
	podpis.setFont(czcionka);
	podpis.setCharacterSize(50);
	podpis.setStyle(sf::Text::Bold);
	podpis.setFillColor(sf::Color::Red);
	podpis.move(300, 0);

	// tworzymy rozgrywke
	//Rozgrywka rozgrywka = prosta_rozgrywka();
	Rozgrywka rozgrywka = zwarcie_rozgrywka(sciezka);
	// przygotowujemy dzialaczy
	Wyswietlacz wyswietlacz(rozgrywka);
	wyswietlacz.Zaladuj("wroclaw");
	MyszDecydent myszkaGracza(window, rozgrywka, rozgrywka.Gracz(nr_gracza));
	OznaczaczWyborow ruchGracza(myszkaGracza);

	//ZMIEN NAZWY GRACZÓW
	if (misja_ustawienia.nazwy_graczow.size())
	{
		for (int i = 0; i < misja_ustawienia.nazwy_graczow.size(); i++)
		{
			rozgrywka.Gracz(i).nazwa = misja_ustawienia.nazwy_graczow[i];
		}
	}
	
	//KOMPUTEROWIE
	vector<Komputer*> kompiutery;
	int nr_kompa = 1;
	for (auto nr : misja_ustawienia.komputery)
	{
		rozgrywka.Gracz(nr).nazwa = "KOMPUTER" + nr;
		if (poziomy_trudnosci[0] == trudnosc)
			kompiutery.emplace_back(new Komputer(rozgrywka, rozgrywka.Gracz(nr)));
		else if (poziomy_trudnosci[1] == trudnosc)
			kompiutery.emplace_back(new KomputerSilver(rozgrywka, rozgrywka.Gracz(nr)));
	}

	ruszacz.rozgrywka = &rozgrywka;
	ruszacz.szybkosc *= predkosc;
	
	//PRYGOTOWANIE ROZGRYWKI
	odliczanie(podpis, window, wyswietlacz);

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
		///FPSY
		czas = (double)(clock() - czasomierz) / CLOCKS_PER_SEC;
		auto& ruchy = myszkaGracza.WykonajRuch();
		ruszacz.PrzyjmijRuch(ruchy);

		for (auto& komp : kompiutery)
		{
			komp->czas += czas;
			auto& ruchy = komp->WykonajRuch();
			ruszacz.PrzyjmijRuch(ruchy);
		}

		ruszacz.Ruszaj(czas);

		window.clear();
		wyswietlacz.WyswietlTlo(window);
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

		ruchGracza.Wyswietlaj(window);
		wyswietlacz.Wyswietlaj(window);
		//ZAKONCZENIE GRY
		if (rozgrywka.liczba_aktywnych_graczy == 1)  // !rozgrywka.Gracz(nr_gracza).aktywny || 
		{
			podpis.setCharacterSize(75);
			podpis.setPosition(100, 200);
			if (rozgrywka.Gracz(nr_gracza).aktywny)podpis.setString("GRATULACJE DLA GRACZA  ");
			else podpis.setString("TYM RAZEM ZWYCIEZYLA SI (LESZCZU!)");
			window.draw(podpis);
			window.display();
			Sleep(3000);
			window.close();
		}
		else
		{
			podpis.setCharacterSize(50);
			podpis.setPosition(1200, 0);
			podpis.setString(std::to_string(rozgrywka.liczba_aktywnych_graczy));
			window.draw(podpis);
		}
		window.display();

		czasomierz = clock();

		Sleep(16);
	}
	return 0;
}

int misja(MisjaUstawienia misja_ustawienia)
{
	Ruszacz ruszacz;
	return misja(misja_ustawienia, ruszacz);
}