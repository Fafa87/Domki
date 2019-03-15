#include "misja.h"
#include "gui.h"

#include <SFML/Audio.hpp>

MisjaUstawienia::MisjaUstawienia() : ile_kto_wygranych(5) {}

int MisjaUstawienia::Zwyciezca()
{
	int res = -1;
	for (int i = 0; i < ile_kto_wygranych.size(); i++)
	{
		if (ile_kto_wygranych[i] >= do_ilu_wygranych)
			return i;
	}
	return res;
}

bool MisjaUstawienia::JedenGracz()
{
	return nazwy_graczow.size() <= 1;
}

void MisjaUstawienia::WypiszRanking()
{
	vector<pair<int,string>> stan;
	for (int i = 0; i < min(ile_kto_wygranych.size(), nazwy_graczow.size()); i++)
	{
		stan.push_back(make_pair(ile_kto_wygranych[i], nazwy_graczow[i]));
	}
	sort(stan.begin(), stan.end(), greater<pair<int, string>>());
	printf("Ranking:\n");
	for (int i = 0; i < stan.size(); i++)
	{
		printf("%s - %d\n", stan[i].second.c_str(), stan[i].first);
	}
}

vector<string> wczytaj_liste_plansz()
{
	return get_all_names_within_folder("Plansza");
}

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
	res.ile_kto_wygranych = vector<int>(5);

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
	gracz0.kolor = sf::Color(128,128,128);
	gracz0.aktywny = false;

	gra.gracze.push_back(Gracz());
	Gracz& gracz1 = gra.gracze.back();
	gracz1.numer = 1; gracz1.nazwa = "GRACZ";
	gracz1.kolor = sf::Color::Red;

	gra.gracze.push_back(Gracz());
	Gracz& gracz2 = gra.gracze.back();
	gracz2.numer = 2; gracz2.nazwa = "CIENIAS";
	gracz2.kolor = sf::Color::Blue;

	gra.gracze.push_back(Gracz());
	Gracz& gracz3 = gra.gracze.back();
	gracz3.numer = 3; gracz3.nazwa = "CZERESNIAK";
	gracz3.kolor = sf::Color::Green;

	gra.gracze.push_back(Gracz());
	Gracz& gracz4 = gra.gracze.back();
	gracz4.numer = 4; gracz4.nazwa = "CWANIACZEK";
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
			else if(parametr == "typ")
				{
				string znak;
				plikmapa >> znak;
				if (znak == "zamek"||znak=="fort")domek.typdomku = TypDomku::kFort;
				else if (znak == "wieza")domek.typdomku = TypDomku::kWieza;
				else if (znak == "kuznia"||znak=="zbrojownia")domek.typdomku = TypDomku::kZbrojownia;
				else if (znak == "stajnia")domek.typdomku = TypDomku::kStajnia;
				}
			else if (parametr == "bez_przebudowy")
				{
				domek.przebudowa = false;
				}
			else if (parametr == "bez_ulepszania")
				{
					domek.ulepszanie = false;
				}
			else if (parametr == "liczebnosc")
				{
					int ile_ludkow;
					plikmapa >> ile_ludkow;
					if (domek.max_liczebnosc == -1)domek.max_liczebnosc = 100;
					gra.ZmienLiczebnosc(domek, ile_ludkow);
				}
			else if (parametr == "poziom")
				{
					int lev;
					plikmapa >> lev;
					domek.poziom = lev;
					domek.max_liczebnosc = 100;
					domek.max_liczebnosc *= 1 << (domek.poziom - 1);
				}
			}
		domek.produkcja = 2;
		if(domek.max_liczebnosc==-1)domek.max_liczebnosc = 100;
		domek.wyglad = Wyglad::kMiasto;
		if (domek.gracz == nullptr)
		{
			domek.gracz = &gracz0;
			if(domek.liczebnosc==-1)gra.ZmienLiczebnosc(domek, 25);
		}
		else if(domek.liczebnosc==-1)gra.ZmienLiczebnosc(domek, 50);
		}
	for (auto para : numery_domkow)
	{
		Domek   *wksslask1,*wksslask2;
		wksslask1 = domki_po_numerze[para.first];
		wksslask2 = domki_po_numerze[para.second];
		wksslask1->drogi.push_back(wksslask2);
		wksslask2->drogi.push_back(wksslask1);
	}

	for (auto& gracz : gra.gracze) if (gracz.numer >= 1)
	{
		gracz.istotny = gracz.liczba_tworow > 0;
		gracz.aktywny = gracz.liczba_tworow > 0;
		if (!gracz.aktywny)
			gra.liczba_aktywnych_graczy--;
	}
	
	plikmapa.close();
	return gra;
}

string ranking_widget_id(int instance, int gracz, string sufix)
{
	return "Ins-" + to_string(instance) + "-Inter-Gracz-" + to_string(gracz) + "-" + sufix;
}

shared_ptr<sfg::Table> interfejs_ranking(MisjaUstawienia &stan, Rozgrywka& rozgrywka, int instance)
{
	auto table = sfg::Table::Create();

	auto gracze = rozgrywka.gracze;
	gracze.sort([stan](const Gracz & a, const Gracz & b) -> bool
	{
		return stan.ile_kto_wygranych[a.numer] > stan.ile_kto_wygranych[b.numer];
	});

	int i = 0;
	for (auto& gracz : gracze)
	{
		auto nr = gracz.numer;
		if (gracz.istotny)
		{
			auto graczId = ranking_widget_id(instance, nr, "");
			auto graczLudkiId = ranking_widget_id(instance, nr, "sila");
			auto graczProdukcjaId = ranking_widget_id(instance, nr, "produkcja");
			auto graczModernizacjaId = ranking_widget_id(instance, nr, "modernizacja");

			// to zajmuje jakos duzo czasu, nie wiem czemu!
			GUI::aplikacja.pulpit.SetProperty<sf::Color>("Button#" + graczId, "BackgroundColor", gracz.kolor);
			GUI::aplikacja.pulpit.SetProperty("Button#" + graczId, "FontSize", 32);
			GUI::aplikacja.pulpit.SetProperty("Label#" + graczProdukcjaId, "Color", gracz.kolor);
			GUI::aplikacja.pulpit.SetProperty("Label#" + graczProdukcjaId, "FontSize", 16);
			GUI::aplikacja.pulpit.SetProperty("Label#" + graczModernizacjaId, "Color", sf::Color::Yellow);
			GUI::aplikacja.pulpit.SetProperty("Label#" + graczModernizacjaId, "FontSize", 16);
			GUI::aplikacja.pulpit.SetProperty("Label#" + graczLudkiId, "FontName", "Grafika/waltographUI.ttf");
			GUI::aplikacja.pulpit.SetProperty("Label#" + graczLudkiId, "FontSize", 24);
			if (stan.Zwyciezca() == nr)
			{
				GUI::aplikacja.pulpit.SetProperty("Button#" + graczId, "FontSize", 80.f);
				GUI::aplikacja.pulpit.SetProperty("Label#" + graczId, "FontSize", 80.f);
			}

			auto wartosc = sfg::Button::Create(to_string(stan.ile_kto_wygranych[nr]));
			wartosc->SetId(graczId);
			auto nazwa = sfg::Label::Create(gracz.nazwa);
			nazwa->SetId(graczId);

			auto sila_gracza = rozgrywka.SilaGracza(nr);
			auto ludki = sfg::Label::Create(to_string(std::get<0>(sila_gracza)));
			ludki->SetId(graczLudkiId);
			auto produkcja = sfg::Label::Create(to_string(std::get<1>(sila_gracza)));
			produkcja->SetId(graczProdukcjaId);
			auto modernizacja = sfg::Label::Create(to_string(std::get<2>(sila_gracza)));
			modernizacja->SetId(graczModernizacjaId);

			table->Attach(wartosc, sf::Rect<sf::Uint32>(0, i, 1, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL, sf::Vector2f(10.f, 10.f));
			table->Attach(nazwa, sf::Rect<sf::Uint32>(1, i, 1, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL, sf::Vector2f(10.f, 10.f));
			table->Attach(ludki, sf::Rect<sf::Uint32>(2, i, 1, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL, sf::Vector2f(10.f, 10.f));
			table->Attach(produkcja, sf::Rect<sf::Uint32>(3, i, 1, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL, sf::Vector2f(10.f, 10.f));
			table->Attach(modernizacja, sf::Rect<sf::Uint32>(4, i, 1, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL, sf::Vector2f(10.f, 10.f));
			i++;
		}
	}

	return table;
}

shared_ptr<sfg::Window> interfejs_rozgrywki(shared_ptr<sfg::Window> interfejs, MisjaUstawienia &stan, Rozgrywka& rozgrywka)
{
	
	if (interfejs == nullptr)
	{
		if (stan.do_ilu_wygranych > 0)
		{
			interfejs = sfg::Window::Create();
			interfejs->SetTitle("Mecz do " + to_string(stan.do_ilu_wygranych) + " wygranych");
			interfejs->SetRequisition(sf::Vector2f(140, 0));

			auto ranking = interfejs_ranking(stan, rozgrywka, 1);

			GUI::aplikacja.pulpit.Add(interfejs);
			interfejs->Add(ranking);
			GUI::aplikacja.bottom_left_window(interfejs);
		}
	}
	else
	{
		for (auto& gracz : rozgrywka.gracze)
		{
			auto nr = gracz.numer;
			if (gracz.istotny)
			{
				auto sila_gracza = rozgrywka.SilaGracza(nr);
				auto sila_label = std::static_pointer_cast<sfg::Label>(interfejs->GetWidgetById(ranking_widget_id(1, nr, "sila")));
				sila_label->SetText((to_string(std::get<0>(sila_gracza))));

				auto produkcja_label = std::static_pointer_cast<sfg::Label>(interfejs->GetWidgetById(ranking_widget_id(1, nr, "produkcja")));
				produkcja_label->SetText((to_string(std::get<1>(sila_gracza))));

				auto modernizacja_label = std::static_pointer_cast<sfg::Label>(interfejs->GetWidgetById(ranking_widget_id(1, nr, "modernizacja")));
				modernizacja_label->SetText((to_string(std::get<2>(sila_gracza))));
			}
		}
	}
	return interfejs;
}

void odliczanie(Wyswietlacz& wyswietlacz)
{
	auto okno = sfg::Window::Create(sfg::Window::Style::BACKGROUND | sfg::Window::Style::SHADOW);
	okno->SetRequisition(sf::Vector2f(500, 300));
	GUI::aplikacja.center_window(okno);

	auto odliczanie_etykieta = sfg::Label::Create("");
	odliczanie_etykieta->SetId("Alarm");
	okno->Add(odliczanie_etykieta);
	GUI::aplikacja.pulpit.Add(okno);

	sf::SoundBuffer pikPikBuffer;
	pikPikBuffer.loadFromFile("Muzyka\\Boom.ogg");

	sf::SoundBuffer pukBuffer;
	pukBuffer.loadFromFile("Muzyka\\Bam.ogg");

	sf::Sound pikPik(pikPikBuffer);
	pikPik.setVolume(30);

	sf::Sound puk(pukBuffer);
	puk.setPitch(2);

	for (int a = 3; a >= 0; a--)
	{
		GUI::aplikacja.okno.clear();
		wyswietlacz.WyswietlTlo(GUI::aplikacja.okno);
		if (a>0) odliczanie_etykieta->SetText(std::to_string(a));
		else odliczanie_etykieta->SetText("RUSZAJ!");
		wyswietlacz.Wyswietlaj(GUI::aplikacja.okno);

		GUI::aplikacja.pulpit.Update(1);
		GUI::aplikacja.render();

		if (a > 0)
		{
			pikPik.play();
			Sleep(1500);
		}
		else
		{
			puk.play();
			Sleep(800);
		}
	}

	GUI::aplikacja.pulpit.Remove(okno);
}

void zakonczenie_gry(Gracz& gracz_wygrany, int grajacy)
{
	auto okno = sfg::Window::Create(sfg::Window::Style::BACKGROUND | sfg::Window::Style::SHADOW);
	okno->SetRequisition(sf::Vector2f(800, 300));

	auto komunikat_koncowy = sfg::Label::Create("");
	komunikat_koncowy->SetId("Naglowek");
	komunikat_koncowy->SetAlignment(sf::Vector2f(0.5, 0.5));
	okno->Add(komunikat_koncowy);
	sf::SoundBuffer oklaskiBuffer;
	oklaskiBuffer.loadFromFile("Muzyka\\Oklaski.ogg");
	sf::Sound oklaski(oklaskiBuffer);

	if (gracz_wygrany.numer == grajacy)
	{
		oklaski.play();
		komunikat_koncowy->SetText("GRATULACJE DLA GRACZA:\n" + gracz_wygrany.nazwa);
	}
	else
		komunikat_koncowy->SetText("TY LESZCZU ZWYCIEZYL:\n" + gracz_wygrany.nazwa);

	GUI::aplikacja.show_and_wait_for_anything(okno);
}

void zakonczenie_meczu(MisjaUstawienia &stan, Rozgrywka& rozgrywka)
{
	auto okno = sfg::Window::Create(sfg::Window::Style::BACKGROUND | sfg::Window::Style::SHADOW | sfg::Window::Style::RESIZE);
	okno->SetRequisition(sf::Vector2f(800, 0));

	auto komunikat_koncowy = sfg::Label::Create("OSTATECZNY RANKING:\n");
	komunikat_koncowy->SetId("Naglowek");
	komunikat_koncowy->SetAlignment(sf::Vector2f(0.5, 0.5));

	auto ranking = interfejs_ranking(stan, rozgrywka, 2);
	
	auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 30.0f);
	box->Pack(komunikat_koncowy);
	box->Pack(ranking);
	
	okno->Add(box);
	GUI::aplikacja.show_and_wait_for_anything(okno);
}

sf::View wysrodkowany_widok(list<Domek> &domki, int bottom_space)
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

	auto powiekszenie_y = min(minimalny_Y, 100.0);
	minimalny_Y -= powiekszenie_y;
	maksymalny_Y += powiekszenie_y;

	auto okno_size = GUI::aplikacja.okno.getSize();

	double stosunek = (double) okno_size.x / (okno_size.y - bottom_space);
	auto dlugosc_Y = maksymalny_Y - minimalny_Y;
	auto dlugosc_X = maksymalny_X - minimalny_X;

	auto dlugosc_X_stosunku = stosunek * dlugosc_Y;
	auto dlugosc_X_z_prop = max(dlugosc_X_stosunku, dlugosc_X);
	auto dlugosc_Y_z_prop = dlugosc_X_z_prop / stosunek;

	minimalny_X += (dlugosc_X - dlugosc_X_z_prop) / 2;

	return sf::View(sf::FloatRect(minimalny_X, minimalny_Y, dlugosc_X_z_prop, dlugosc_Y_z_prop));
}

int misja(MisjaUstawienia& misja_ustawienia, Ruszacz& ruszacz)
{
	string sciezka = join(misja_ustawienia.grupa, misja_ustawienia.nazwa);
	string trudnosc = misja_ustawienia.trudnosc;
	double predkosc = misja_ustawienia.szybkosc;
	int nr_gracza = misja_ustawienia.nr_gracza;
	bool przyspieszenie_bez_gracza = false;

	sf::RenderWindow& window = GUI::aplikacja.okno;

	if (misja_ustawienia.nr_gracza == 0)
		window.setVisible(false);

	// tworzymy rozgrywke
	Rozgrywka rozgrywka = zwarcie_rozgrywka(sciezka);
	rozgrywka.walka_w_polu = misja_ustawienia.walka_w_polu;
	// przygotowujemy dzialaczy
	Wyswietlacz wyswietlacz(rozgrywka);
	wyswietlacz.Zaladuj(misja_ustawienia.skorka);
	MyszDecydent myszkaGracza(window, rozgrywka, rozgrywka.Gracz(nr_gracza));
	OznaczaczWyborow ruchGracza(myszkaGracza);
	Muzykant muzykant(rozgrywka);
	muzykant.Zaladuj(misja_ustawienia.skorka);

	sf::View view = wysrodkowany_widok(rozgrywka.domki);
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
		//rozgrywka.Gracz(nr).nazwa = "KOMPUTER" + nr;
		if (poziomy_trudnosci[0] == trudnosc)
			kompiutery.emplace_back(new Komputer(rozgrywka, rozgrywka.Gracz(nr),misja_ustawienia.szybkosc));
		else if (poziomy_trudnosci[1] == trudnosc)
			kompiutery.emplace_back(new KomputerSilver(rozgrywka, rozgrywka.Gracz(nr),misja_ustawienia.szybkosc));
	}

	ruszacz.rozgrywka = &rozgrywka;
	ruszacz.szybkosc *= predkosc;
	
	odliczanie(wyswietlacz);

	muzykant.Przygrywaj();

	//czasomierz
	auto czasomierz = clock();
	int czasik = 0;
	double czas;
	//APM
	long long akcje = 0;
	double czas_przeminal = clock();
	shared_ptr<sfg::Window> interfejs;

	bool trwa_gra = true;
	while (trwa_gra)
	{
		sf::Event event;

		myszkaGracza.Przetworz(); // puste
		while (window.pollEvent(event))
		{
			GUI::aplikacja.pulpit.HandleEvent(event);
			myszkaGracza.Przetworz(event);
			if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::KeyReleased)akcje++;
			switch (event.type)
			{
			case sf::Event::KeyReleased:
				switch (event.key.code)
				{
				case sf::Keyboard::Escape:
					// nie startuj kolejnych gier
					misja_ustawienia.ile_kto_wygranych[0] = 100;
					trwa_gra = false;
					break;
				case sf::Keyboard::F3:
					wyswietlacz.ZaladujInne();
					break;
				}
				break;
			}
			if (event.type == sf::Event::Closed)
				trwa_gra = false;
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
		muzykant.GrajEfekty(ruszacz);

		// przyspiesz jesli zostaly same komputery
		if (!przyspieszenie_bez_gracza && misja_ustawienia.JedenGracz() && !rozgrywka.Gracz(misja_ustawienia.nr_gracza).aktywny)
		{
			int mnoznik_czasu = 5;
			for (auto komp : kompiutery)
				komp->szybkosc_komputera *= mnoznik_czasu;
			ruszacz.szybkosc *= mnoznik_czasu;
			przyspieszenie_bez_gracza = true;
		}

		window.clear();
		wyswietlacz.WyswietlTlo(window);

		interfejs = interfejs_rozgrywki(interfejs, misja_ustawienia, rozgrywka);

		czasik = (int)(1.0 / czas + 0.5);
		czas_przeminal = (double)(clock() - czas_przeminal) / CLOCKS_PER_SEC;

		ruchGracza.Wyswietlaj(window);
		wyswietlacz.Wyswietlaj(window);

		//ZAKONCZENIE GRY
		if (rozgrywka.liczba_aktywnych_graczy == 1)
		{
			muzykant.Zamilcz();
			for (auto& g : rozgrywka.gracze)
			{
				if (g.aktywny)
				{
					misja_ustawienia.ile_kto_wygranych[g.numer]++;
					if (misja_ustawienia.Zwyciezca() < 0 && misja_ustawienia.nr_gracza != 0)
						zakonczenie_gry(g, nr_gracza);
					break;
				}
			}

			auto zwyciezca_meczu = misja_ustawienia.Zwyciezca();
			if (zwyciezca_meczu >= 0 && misja_ustawienia.nr_gracza != 0)
			{
				zakonczenie_meczu(misja_ustawienia, rozgrywka);
			}
			trwa_gra = false;
			break;
		}

		GUI::aplikacja.render();

		Sleep(16);
	}

	GUI::aplikacja.reset_view();

	if(interfejs != nullptr)
		GUI::aplikacja.pulpit.Remove(interfejs);
	return 0;
}

int misja(MisjaUstawienia& misja_ustawienia)
{
	Ruszacz ruszacz;
	return misja(misja_ustawienia, ruszacz);
}