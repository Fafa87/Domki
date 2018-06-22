#include "misja.h"
#include "gui.h"


MisjaUstawienia wczytaj_meta(string sciezka)
{
	MisjaUstawienia res;
	res.komputery.clear();

	ifstream plikmapa;
	plikmapa.open(sciezka);
	int liczba_domkow, numer_domku, liczba_parametrow;
	string parametr;
	plikmapa >> liczba_domkow;

	// wyznacz liczbę graczy na apie
	while (plikmapa.good())
	{
		plikmapa >> parametr;
		if (parametr == "gracz")
		{
			int num;
			plikmapa >> num;
			res.komputery.push_back(num);
		}
	}

	sort(res.komputery.begin(), res.komputery.end());
	res.komputery.erase(unique(res.komputery.begin(), res.komputery.end()), res.komputery.end());
	res.komputery.erase(res.komputery.begin()); // usun pierwszego

	plikmapa.close();

	return res;
}

Rozgrywka zwarcie_rozgrywka(string sciezka)
{
	Rozgrywka gra;
	//gracze
	gra.gracze.push_back(Gracz());
	Gracz& gracz0 = gra.gracze.back();
	gracz0.numer = 0; gracz0.nazwa = "NEUTRAL";
	gracz0.kolor = sf::Color::White;
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

	gra.liczba_aktywnych_graczy = 4;
	//domki
	ifstream plikmapa;
	plikmapa.open(sciezka);
	int liczba_domkow, numer_domku, liczba_parametrow;
	string parametr;
	plikmapa >> liczba_domkow;

	//KOORDYNATY , DROGI , GRACZ

	map<int, Domek*> domki_po_numerze;
	vector<std::pair<int,int>> numery_domkow;
	for (int a = 0; a < liczba_domkow; a++)
		{
		plikmapa >> numer_domku >> liczba_parametrow;
		gra.domki.push_back(Domek());
		Domek& domek = gra.domki.back();
		domki_po_numerze[numer_domku] = &domek;
		for (int b = 0; b < liczba_parametrow; b++)
			{
			plikmapa >> parametr;
			if (parametr == "koordynaty")
				{
				double x, y;
				plikmapa >> x >> y;
				domek.polozenie = {x,y};
				}
			else if (parametr == "drogi")
				{
				int ile,numer;
				plikmapa >> ile;
				for (int c = 0; c < ile; c++)
					{
					plikmapa >> numer;
					numery_domkow.push_back(make_pair(numer_domku, numer));
					}
				}
			else if (parametr == "gracz")
				{
				char znak;
				plikmapa >> znak;
				if (znak == '1')domek.gracz = &gracz1;
				else if (znak == '2')domek.gracz = &gracz2;
				else if (znak == '3')domek.gracz = &gracz3;
				else if (znak == '4')domek.gracz = &gracz4;
				if (znak == '1')gracz1.liczba_tworow++;
				else if (znak == '2')gracz2.liczba_tworow++;
				else if (znak == '3')gracz3.liczba_tworow++;
				else if (znak == '4')gracz4.liczba_tworow++;
				}
			}
		domek.produkcja = 2;
		domek.max_liczebnosc = 100;
		domek.wyglad = Wyglad::kDomek;
		if (domek.gracz == nullptr)
		{
			domek.gracz = &gracz0;
			gra.ZmienLiczebnosc(domek, 25);
		}
		else gra.ZmienLiczebnosc(domek, 50);
		}
	for (auto para : numery_domkow)
	{
		Domek   *wksslask1,*wksslask2;
		wksslask1 = domki_po_numerze[para.first];
		wksslask2 = domki_po_numerze[para.second];
		wksslask1->drogi.push_back(wksslask2);
		wksslask2->drogi.push_back(wksslask1);
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

void odliczanie(sf::RenderWindow& window, Wyswietlacz& wyswietlacz)
{
	auto okno = sfg::Window::Create(sfg::Window::Style::BACKGROUND | sfg::Window::Style::SHADOW);
	okno->SetRequisition(sf::Vector2f(500, 300));
	GUI::center_window(window, okno);

	auto odliczanie_etykieta = sfg::Label::Create("");
	odliczanie_etykieta->SetId("Alarm");
	okno->Add(odliczanie_etykieta);
	GUI::pulpit.Add(okno);

	for (int a = 3; a >= 0; a--)
	{
		window.clear();
		wyswietlacz.WyswietlTlo(window);
		if (a>0) odliczanie_etykieta->SetText(std::to_string(a));
		else odliczanie_etykieta->SetText("RUSZAJ!");
		wyswietlacz.Wyswietlaj(window);

		GUI::pulpit.Update(1);
		GUI::sfgui.Display(window);

		window.display();
		if (a > 0)Sleep(1000);
		else Sleep(250);
	}

	GUI::pulpit.Remove(okno);
}

void zakonczenie_gry(sf::RenderWindow& window, Gracz& gracz_wygrany, int grajacy)
{
	auto okno = sfg::Window::Create(sfg::Window::Style::BACKGROUND | sfg::Window::Style::SHADOW);
	okno->SetRequisition(sf::Vector2f(800, 300));

	auto komunikat_koncowy = sfg::Label::Create("");
	komunikat_koncowy->SetId("Naglowek");
	komunikat_koncowy->SetAlignment(sf::Vector2f(0.5, 0.5));
	okno->Add(komunikat_koncowy);
	GUI::pulpit.Add(okno);

	if (gracz_wygrany.numer == grajacy)
		komunikat_koncowy->SetText("GRATULACJE DLA GRACZA " + gracz_wygrany.nazwa);
	else
		komunikat_koncowy->SetText("TYM RAZEM ZWYCIEZYL " + gracz_wygrany.nazwa + "\n		LESZCZU!");

	GUI::center_window(window, okno);

	GUI::pulpit.Update(1);
	GUI::sfgui.Display(window);
	window.display();
	
	Sleep(4000);
	GUI::pulpit.Remove(okno);
}

sf::View WysrodkowanyWidok(list<Domek> &domki)
{
	auto minimalny_X = 10000.0, maksymalny_X = 0.0;
	auto minimalny_Y = 10000.0, maksymalny_Y = 0.0;

	for (auto& dom : domki)
	{
		minimalny_X = min(minimalny_X, (double)dom.polozenie.x);
		maksymalny_X = max(maksymalny_X, (double)dom.polozenie.x);

		minimalny_Y = min(minimalny_Y, (double)dom.polozenie.y);
		maksymalny_Y = max(maksymalny_Y, (double)dom.polozenie.y);
	}
	
	auto powiekszenie_x = min(minimalny_X, 100.0);
	minimalny_X -= powiekszenie_x;
	maksymalny_X += powiekszenie_x;

	auto powiekszenie_y = min(minimalny_Y, 80.0);
	minimalny_Y -= powiekszenie_y;
	maksymalny_Y += powiekszenie_y;

	double stosunek = 1600 / 899.0;
	auto dlugosc_Y = maksymalny_Y - minimalny_Y;
	auto dlugosc_X = maksymalny_X - minimalny_X;

	auto dlugosc_X_stosunku = stosunek * dlugosc_Y;
	auto dlugosc_X_z_prop = max(dlugosc_X_stosunku, dlugosc_X);
	auto dlugosc_Y_z_prop = dlugosc_X_z_prop / stosunek;

	minimalny_X += (dlugosc_X - dlugosc_X_z_prop) / 2;

	return sf::View(sf::FloatRect(minimalny_X, minimalny_Y, dlugosc_X_z_prop, dlugosc_Y_z_prop));
}

int misja(MisjaUstawienia misja_ustawienia, Ruszacz& ruszacz)
{
	string sciezka = "Plansza\\" + misja_ustawienia.nazwa;
	string trudnosc = misja_ustawienia.trudnosc;
	double predkosc = misja_ustawienia.szybkosc;
	int nr_gracza = misja_ustawienia.nr_gracza;

	sf::ContextSettings ustawienia;
	ustawienia.antialiasingLevel = 8;

	auto videoMode = sf::VideoMode(1600, 899);
	sf::RenderWindow window(videoMode, "DOMKI PRE-ALFA!", sf::Style::None, ustawienia); // sf::Style::Fullscreen

	if (misja_ustawienia.nr_gracza == 0)
		window.setVisible(false);

	// fpsy
	sf::Font czcionka;
	czcionka.loadFromFile("Grafika\\waltographUI.ttf");

	/*sf::Text podpis;
	podpis.setFont(czcionka);
	podpis.setCharacterSize(50);
	podpis.setStyle(sf::Text::Bold);
	podpis.setFillColor(sf::Color::Red);
	podpis.move(400, 0);*/

	// tworzymy rozgrywke
	//Rozgrywka rozgrywka = prosta_rozgrywka();
	Rozgrywka rozgrywka = zwarcie_rozgrywka(sciezka);
	rozgrywka.walka_w_polu = misja_ustawienia.walka_w_polu;
	// przygotowujemy dzialaczy
	Wyswietlacz wyswietlacz(rozgrywka);
	wyswietlacz.Zaladuj(misja_ustawienia.skorka);
	MyszDecydent myszkaGracza(window, rozgrywka, rozgrywka.Gracz(nr_gracza));
	OznaczaczWyborow ruchGracza(myszkaGracza);

	sf::View view = WysrodkowanyWidok(rozgrywka.domki);
	window.setView(view);

	//ZMIEN NAZWY GRACZ�W
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
			kompiutery.emplace_back(new Komputer(rozgrywka, rozgrywka.Gracz(nr),misja_ustawienia.szybkosc));
		else if (poziomy_trudnosci[1] == trudnosc)
			kompiutery.emplace_back(new KomputerSilver(rozgrywka, rozgrywka.Gracz(nr),misja_ustawienia.szybkosc));

		
	}

	ruszacz.rozgrywka = &rozgrywka;
	ruszacz.szybkosc *= predkosc;
	
	odliczanie(window, wyswietlacz);

	//czasomierz
	auto czasomierz = clock();
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
		czasomierz = clock();
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
		

		czasik = (int)(1.0 / czas + 0.5);
		///FPSY
		//podpis.setString("FPSY: " + std::to_string(czasik));
		//podpis.setPosition(300, 0);
		//window.draw(podpis);//FPSY

		///APM
		//podpis.setPosition(700, 0);
		czas_przeminal = (double)(clock() - czas_przeminal) / CLOCKS_PER_SEC;
		//podpis.setString("APM: " + std::to_string((int)(60 * akcje / czas_przeminal)));
		//window.draw(podpis); //APM

		ruchGracza.Wyswietlaj(window);
		wyswietlacz.Wyswietlaj(window);

		//ZAKONCZENIE GRY
		if (rozgrywka.liczba_aktywnych_graczy == 1)  // !rozgrywka.Gracz(nr_gracza).aktywny || 
		{
			for (auto& g : rozgrywka.gracze)
			{
				if (g.aktywny)
					zakonczenie_gry(window, g, nr_gracza);
			}
			//podpis.setCharacterSize(75);
			//podpis.setPosition(300, 200);
			window.close();
		}
		/*else
		{
			podpis.setCharacterSize(50);
			podpis.setPosition(1200, 0);
			podpis.setString(std::to_string(rozgrywka.liczba_aktywnych_graczy));
			window.draw(podpis);
		}*/
		window.display();

		Sleep(16);
	}
	return 0;
}

int misja(MisjaUstawienia misja_ustawienia)
{
	Ruszacz ruszacz;
	return misja(misja_ustawienia, ruszacz);
}