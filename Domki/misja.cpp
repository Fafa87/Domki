#include "misja.h"
#include "gui.h"

#include "ext_string.h"
#include <SFML/Audio.hpp>

#include "easylogging++.h"

#include <cstdlib>
#include <ctime>

#include <locale>
#include <codecvt>
#include <string>
#include <filesystem>

MisjaUstawienia::MisjaUstawienia() : ile_kto_wygranych(16) {}

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

vector<string> wczytaj_liste_plansz(string folder)
{
    return get_all_names_within_folder(folder);
}

vector<string> wczytaj_liste_folderow(string folder)
{
    return get_all_names_within_folder(folder,1);
}

MisjaUstawienia wczytaj_meta(string sciezka)
{
    MisjaUstawienia res;
    res.komputery.clear();

    ifstream plikmapa;
    plikmapa.open(sciezka);

    int liczba_domkow; // , numer_domku, liczba_parametrow;
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
    res.ile_kto_wygranych = vector<int>(13);

    plikmapa.close();

    return res;
}

void zapis_mapy(const Rozgrywka& konstrukcja, string sciezka) {
    ofstream plikmapy;
    plikmapy.open(sciezka);

    int liczba_domkow = (int)konstrukcja.domki.size(); // , nr_domku;

    plikmapy << liczba_domkow << "\n";

    map <Domek*, int> numery_domkow;
    map <int, Domek*> domki_o_numerach;

	map <int, int> numerki;
    int nr = 1;
    for (Domek domek : konstrukcja.domki) {
        //numery_domkow[&domek] = nr;
        //domki_o_numerach[nr] = &domek;
		numerki[domek.uid] = nr;
        nr++;
    }

    //for (int nr = 1; nr <= liczba_domkow; nr++) {
	for(Domek domek : konstrukcja.domki){
        
        int liczba_parametrow = 1;
        
        if (domek.drogi.size()) liczba_parametrow++;
        if (domek.gracz->numer) liczba_parametrow++;
        if (domek.typdomku != TypDomku::kMiasto) liczba_parametrow++;
        if (domek.liczebnosc) liczba_parametrow++;
        if (!domek.przebudowa) liczba_parametrow++;
        if (!domek.ulepszanie) liczba_parametrow++;
        if (!domek.kontrola) liczba_parametrow++;
        if (domek.poziom != 1) liczba_parametrow++;

        plikmapy << numerki[domek.uid] << " " << liczba_parametrow << "\n"
             << "koordynaty " << domek.polozenie.x << " " << domek.polozenie.y << "\n";

        if (domek.drogi.size()) {
            plikmapy << "drogi " << domek.drogi.size() << " ";
            vector<int> numery;
			for (Domek* polaczony : domek.drogi)
				//numery.push_back(numery_domkow[polaczony]);
				numery.push_back(numerki[polaczony->uid]);

            sort(numery.begin(), numery.begin());

             for (int i : numery)
                    plikmapy << i << " ";
            plikmapy << "\n";
        }

        if (domek.gracz->numer) {
            plikmapy << "gracz " << domek.gracz->numer << "\n";
        }

        if (domek.typdomku != TypDomku::kMiasto) {
            plikmapy << "typ ";
            switch (domek.typdomku) {
            case TypDomku::kFort: plikmapy << "fort\n"; break;
            case TypDomku::kWieza: plikmapy << "wieza\n"; break;
            case TypDomku::kZbrojownia: plikmapy << "zbrojownia\n"; break;
            case TypDomku::kStajnia: plikmapy << "fort\n"; break;
            case TypDomku::kPole: plikmapy << "pole\n"; break;
            default: plikmapy << "miasto\n";
            }
        }

        if (domek.liczebnosc) {
            plikmapy << "liczebnosc " << domek.liczebnosc << "\n";
        }

        if (!domek.przebudowa) plikmapy << "bez_przebudowy\n";
        if (!domek.ulepszanie) plikmapy << "bez_ulepszania\n";
        if (!domek.kontrola) plikmapy << "bez_kontroli\n";

        if(domek.poziom != 1) {
            plikmapy << "poziom " << domek.poziom << "\n";
        }
    }

    plikmapy.close();
}

Rozgrywka zwarcie_rozgrywka(string sciezka)
{
    Rozgrywka gra;
    //gracze
    sf::Color kolory[] = { sf::Color(255,255,255) , sf::Color(255,0,0), sf::Color(0,0,255), sf::Color(0,255,0),      // 0 - bialy, 1 - czerwony, 2 - niebieski, 3 - zielony
    sf::Color(255,255,0) , sf::Color(255,128,0), sf::Color(0,255,255), sf::Color(127,0,255),                              // 4 - zolty, 5 - pomaranczowy, 6 - jasnoniebieski, 7- fioletowy
    sf::Color(255,0,255) , sf::Color(102,0,51), sf::Color(102,102,0), sf::Color(51,25,0),                        // 8 - rozowy, 9 - bordowy, 10 - ciemnozloty, 11 - brazowy
    sf::Color(255,255,255), };                                                                                                                             // 12 - bialy

    for (int a = 0; a <= 12 ; a++)
    {
        gra.gracze.push_back(Gracz());
        Gracz& gracz = gra.gracze.back();
        gracz.numer = a;
        if (a == 0)gracz.aktywny = false;
        else if (a == 1)gracz.nazwa = "GRACZ";
        gracz.kolor = kolory[a];
    }

    gra.liczba_aktywnych_graczy = 12;
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
                    if(numer_domku != numer)
                        numery_domkow.push_back(make_pair(numer_domku, numer));
                    }
                }
            else if (parametr == "gracz")
                {
                int numer;
                plikmapa >> numer;
                domek.gracz = &gra.Graczu(numer);
                gra.Graczu(numer).liczba_tworow++;
                }
            else if(parametr == "typ")
                {
                string znak;
                plikmapa >> znak;
                if (znak == "zamek"||znak=="fort")domek.typdomku = TypDomku::kFort;
                else if (znak == "wieza")domek.typdomku = TypDomku::kWieza;
                else if (znak == "kuznia"||znak=="zbrojownia")domek.typdomku = TypDomku::kZbrojownia;
                else if (znak == "stajnia")domek.typdomku = TypDomku::kStajnia;
                else if (znak == "pole")domek.typdomku = TypDomku::kPole;
                }
            else if (parametr == "bez_przebudowy")
                {
                domek.przebudowa = false;
                }
            else if (parametr == "bez_ulepszania")
                {
                    domek.ulepszanie = false;
                }
            else if (parametr == "bez_kontroli")
            {
                domek.kontrola = false;
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
                    gra.ZmienPoziom(domek, lev);
                }
            }

        domek.produkcja = 2;
        if(domek.max_liczebnosc==-1)domek.max_liczebnosc = 100;
        domek.wyglad = Wyglad::kMiasto;
        if (domek.gracz == nullptr)
        {
            domek.gracz = &gra.Graczu(0);
            if(domek.liczebnosc==-1)gra.ZmienLiczebnosc(domek, 25);
        }
        else if(domek.liczebnosc==-1)gra.ZmienLiczebnosc(domek, 50);
        }
    if (plikmapa.good()) {
        plikmapa >> parametr;
        if (parametr == "Tryb")
        {
            do {
                    string nazwa_celu;
                    plikmapa >> nazwa_celu;
                    if (nazwa_celu == "bez_ulepszania") {
                        for (Domek& domek : gra.domki) domek.ulepszanie = false;
                    }
                    else  if (nazwa_celu == "bez_przebudowy") {
                        for (Domek& domek : gra.domki) domek.przebudowa = false;
                    }
                    else  if (nazwa_celu == "bez_kontroli") {
                        for (Domek& domek : gra.domki) domek.kontrola = false;
                    }
                    else if (nazwa_celu == "Zbieranie") {
                        plikmapa >> gra.cel_gry.wymagany_zbior;
                    }
                    else if (nazwa_celu == "Inwazja") {
                        plikmapa >> gra.cel_gry.nr_impostera >> gra.cel_gry.szybkosc_impostera;
                    }
                    else if (nazwa_celu == "Przetrwanie") {
                        plikmapa >> gra.cel_gry.nr_defensora >> gra.cel_gry.sila_plagi;
                    }
                    else if (nazwa_celu == "KOTH") {
                        plikmapa >> gra.cel_gry.do_zdobycia;
                    }
                    else {
                        nazwa_celu = "Walka";
                    }
                gra.cel_gry.nazwa_celu = nazwa_celu;
            } while (plikmapa.good());
        }
    }
    for (auto para : numery_domkow)
    {
        Domek   *wksslask1,*wksslask2;
        wksslask1 = domki_po_numerze[para.first];
        wksslask2 = domki_po_numerze[para.second];
		if(find(wksslask1->drogi.begin(), wksslask1->drogi.end(), wksslask2) != wksslask1->drogi.end() || 
		   find(wksslask2->drogi.begin(), wksslask2->drogi.end(), wksslask1) != wksslask2->drogi.end()) //droga wczytywana wystepuje 2 razy
			continue;
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

string map_generator(int liczba_graczy, string &name, string &date) {
    const string names[3][10] = { {"Super", "Ekstra", "SuperDuper", "Superb", "Hoho", "Okazjonalne", "Zachlanne", "Uknute", "Totalne", "Ostre"},
                                {"Starcie", "Miazgi", "Kupy", "Masy", "Pojedynki", "Walki", "Maskarady", "Cyrki", "Mordobicie", "Konfrontacje"},
                                {"Tytanow", "Kozakow", "Krolow", "Leszczy", "Szefow", "Kup", "Szczawi", "Szczurkow", "Bykow", "Czeresniakow"}};
    Rozgrywka gra;
    time_t currentTime = time(0);
    tm* timer = localtime(&currentTime);
    string sciezka_gry;
    date = to_string(timer->tm_mday) + "-" + to_string(timer->tm_mon + 1) + "-" + to_string(timer->tm_year + 1900) + " " + to_string(timer->tm_hour) + to_string(timer->tm_min) + to_string(timer->tm_sec);
    //system(("new-item " + date + " -ItemType Directory").c_str());
    ifstream test;
    srand(time(NULL));
    int name1 = rand() % 10, name2 = name1 * rand() % 10, name3 = name2 * rand() % 10;
    do {
        name1 = (name1 + 1) % 10, name2 = (name2 + 3) % 10, name3 = (name3 + 5) % 10;
        name = "'" + names[0][name1] + " " + names[1][name2] + " " + names[2][name3] + "'";
        sciezka_gry = "Plansza\\Generator\\" + date + " - " + name + ".txt";
        test.open(sciezka_gry);
    }while (test.good());

    sf::Color kolory[] = { sf::Color(255,255,255) , sf::Color(255,0,0), sf::Color(0,0,255), sf::Color(0,255,0),      // 0 - bialy, 1 - czerwony, 2 - niebieski, 3 - zielony
    sf::Color(255,255,0) , sf::Color(255,128,0), sf::Color(0,255,255), sf::Color(127,0,255),                              // 4 - zolty, 5 - pomaranczowy, 6 - jasnoniebieski, 7- fioletowy
    sf::Color(255,0,255) , sf::Color(102,0,51), sf::Color(102,102,0), sf::Color(51,25,0),                        // 8 - rozowy, 9 - bordowy, 10 - ciemnozloty, 11 - brazowy
    sf::Color(255,255,255), };                                                                                                                             // 12 - bialy

    for (int a = 0; a <= 12; a++)
    {
        gra.gracze.push_back(Gracz());
        Gracz& gracz = gra.gracze.back();
        gracz.numer = a;
        if (a == 0)gracz.aktywny = false;
        else if (a == 1)gracz.nazwa = "GRACZ";
        gracz.kolor = kolory[a];
    }


    const int liczba_domkow_do_wygenerowania = 25;
    int wygenerowane = 0;

    srand(time(NULL));
    double x = rand() % 13 + 1, y = (int)x * rand() % 8 + 1;
    while (wygenerowane < liczba_domkow_do_wygenerowania) {
        
        gra.domki.push_back(Domek());
        Domek& domek = gra.domki.back();
        domek.produkcja = 2;
        domek.max_liczebnosc = 100;
        domek.wyglad = Wyglad::kMiasto;
        gra.ZmienLiczebnosc(domek, 50);
        gra.ZmienPoziom(domek, 1);
        do {
            x = (int)y * rand() % 13 + 1, y = (int)x * rand() % 8 + 1;
        } while (gra.Zlokalizuj(100.0 * x, 100.0 * y) != nullptr || gra.CzyTamJestDroga(100.0 * x, 100.0 * y, domek.rozmiar));
        domek.polozenie = { x * 100.0, y * 100.0};
        
        wygenerowane++;
        domek.gracz = &gra.Graczu(0);

        if (wygenerowane > 1) {
            auto iterator = gra.domki.end();
            iterator--;
            while(iterator != gra.domki.begin() && !gra.CzyMoznaPolaczycDomki(domek, (*iterator))) iterator--;
            if (iterator == gra.domki.begin() && !gra.CzyMoznaPolaczycDomki(domek, (*iterator))) continue;
            domek.drogi.push_back(&*iterator);
            (*iterator).drogi.push_back(&domek);
        }
    }

    for (int gracz = 1; gracz <= liczba_graczy; gracz++) {
        int liczba_domkow_gracza = (liczba_graczy > 7 ? 1 : liczba_graczy > 3 ? 2 : 3), nr_domku = (int)x * rand() % gra.domki.size();
        while (liczba_domkow_gracza > 0) {
            while (gra.Domku(nr_domku).gracz != &gra.Graczu(0)) nr_domku = (nr_domku + 1) * rand() % gra.domki.size();
            gra.Domku(nr_domku).gracz = &gra.Graczu(gracz);
            gra.Graczu(gracz).liczba_tworow++;
            liczba_domkow_gracza--;
        }
    }

    for (auto& domek : gra.domki) {
        do {
            int domek_nr = rand() % gra.domki.size();
            auto iterator = gra.domki.begin();
            while (domek_nr) {
                iterator++;
                domek_nr--;
            }
            if ((*iterator).uid != domek.uid && gra.CzyMoznaPolaczycDomki(domek, (*iterator))) {
                domek.drogi.push_back(&*iterator);
                (*iterator).drogi.push_back(&domek);
            }
        } while (domek.drogi.size() == 0);
    }


    zapis_mapy(gra, sciezka_gry);
    return sciezka_gry;
}

bool odliczanie(int czas)
{
    for (int a = czas; a >= 0; a--)
    {
        Sleep(1000);
    }
    return true;
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

    auto okno_size = GUI::aplikacja().okno.getSize();

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
    double trudnosc = misja_ustawienia.trudnosc;
    double predkosc = misja_ustawienia.szybkosc;
    int nr_gracza = misja_ustawienia.nr_gracza;
    bool przyspieszenie_bez_gracza = false;

    sf::RenderWindow& window = GUI::aplikacja().okno;

    bool to_serwer = nr_gracza == 0;

    if (to_serwer)
    {
        window.setVisible(false);
        GUI::aplikacja().dzwieki_glosnosc = 0;
    }

    // tworzymy rozgrywke
    Rozgrywka rozgrywka = zwarcie_rozgrywka(sciezka);
    rozgrywka.oszustwa = misja_ustawienia.oszustwa;
    rozgrywka.walka_w_polu = misja_ustawienia.walka_w_polu;
    rozgrywka.punkty_kontrolne = misja_ustawienia.punkty_kontrolne;

    //ustawiamy tryb gry
    //rozgrywka.cel_gry = misja_ustawienia.cel_misji;///////////////////////////////////////////////////////////////////////

    // przygotowujemy dzialaczy
    Wyswietlacz wyswietlacz(rozgrywka);
    if (!to_serwer)
        wyswietlacz.Zaladuj(misja_ustawienia.skorka);
    MyszDecydent myszkaGracza(window, rozgrywka, rozgrywka.Graczu(nr_gracza));
    OznaczaczWyborow ruchGracza(myszkaGracza);
    Muzykant muzykant(rozgrywka);
    muzykant.Zaladuj(misja_ustawienia.skorka);

    //ZMIEN NAZWY GRACZ�W
    if (misja_ustawienia.nazwy_graczow.size())
    {
        for (int i = 0; i < misja_ustawienia.nazwy_graczow.size(); i++)
        {
            rozgrywka.Graczu(i).nazwa = misja_ustawienia.nazwy_graczow[i];
        }
    }

    srand(time(NULL));
    //KOMPUTEROWIE
    vector<Komputer*> kompiutery;
    int numba = 1, licznik_nazw[3] = {};
    for (int nr = 0; nr < misja_ustawienia.komputery.size(); nr++)numba *= 3;
    numba = rand() % numba;
    for (auto nr : misja_ustawienia.komputery)
    {
        //LOSOWANIE NAZW
        while (nr > 0 &&( !(licznik_nazw[numba % 3] <= licznik_nazw[(numba + 1) % 3] && licznik_nazw[numba % 3] <= licznik_nazw[(numba - 1) % 3]))) numba = rand() % (numba + 2);
        rozgrywka.Graczu(nr).nazwa = misja_ustawienia.nazwy[numba%3][(int)trudnosc-1];
        licznik_nazw[numba % 3]++;
        numba /= 3;
        kompiutery.emplace_back(new Komputer(rozgrywka,rozgrywka.Graczu(nr),trudnosc));
         }

    bool gotowe_do_rozpoczecia = false;

    ruszacz.rozgrywka = &rozgrywka;
    ruszacz.szybkosc *= predkosc;
    
    shared_ptr<sfg::Window> interfejs;
    sf::View view;
    if (!to_serwer)
    {
        interfejs = interfejs_rozgrywki(nullptr, misja_ustawienia, rozgrywka, wyswietlacz, nullptr, nullptr);
        if (interfejs == nullptr)
        {
            LOG(WARNING) << "Nie utworzyl sie interfejs." << "Liczba domków=" << rozgrywka.domki.size();
            gotowe_do_rozpoczecia = false; 
        }
        else
        {
            view = wysrodkowany_widok(rozgrywka.domki, interfejs->GetAllocation().height);
            window.setView(view);
            if (!GUI::aplikacja().ini.GetBoolean("przelaczniki", "pomin_odliczanie", true))
            {
                gotowe_do_rozpoczecia = odliczanie(4, wyswietlacz, view, interfejs, ruszacz);
                if (!gotowe_do_rozpoczecia)
                    LOG(WARNING) << "Nie doczekal sie rozpoczecia na serwerze.";
            }
            else
                gotowe_do_rozpoczecia = true;
        }
    }
    else
    {
        if(!GUI::aplikacja().ini.GetBoolean("przelaczniki", "pomin_odliczanie", true))
            gotowe_do_rozpoczecia = odliczanie(5);
        else
            gotowe_do_rozpoczecia = true;
    }

    if (!GUI::aplikacja().dzwieki_glosnosc)
        muzykant.wyciszony = true;
    
    muzykant.Przygrywaj();

    // Czasomierz i FPS
    auto czasomierz = clock();
    double czas;
    double fpsy;
    int ostatni_pokaz_fps = 0;

    // APM
    long long akcje = 0;
    double start_gry = clock();
    double czas_gry = 0;

    bool trwa_gra = true;

    if (gotowe_do_rozpoczecia)
    while (trwa_gra)
    {
        sf::Event event;

        muzykant.NoGraj();
        myszkaGracza.Przetworz(); // puste
        while (window.pollEvent(event))
        {
            GUI::aplikacja().pulpit.HandleEvent(event);
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
            {
                throw std::exception("okno zamkniete");
            }
        }
        ///FPSY
        czas = (double)(clock() - czasomierz) / CLOCKS_PER_SEC;
        fpsy = (int)(1 / czas + 0.5);

        if (czas_gry - ostatni_pokaz_fps > 2 && GUI::aplikacja().ini.GetBoolean("przelaczniki", "fps", false))
        {
            printf("FPS: %d\n", fpsy);
            LOG(INFO) << "FPS: " << fpsy;
            ostatni_pokaz_fps = (int)czas_gry;
        }

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
        myszkaGracza.Skupienie();
        muzykant.GrajEfekty(ruszacz);

        window.clear();

        if (!to_serwer)
        {
            interfejs = interfejs_rozgrywki(interfejs, misja_ustawienia, rozgrywka, wyswietlacz, ruchGracza.WybranyDomek(), myszkaGracza.skupiony);
            GUI::aplikacja().show_bottom_gui(view, interfejs);
            wyswietlacz.WyswietlTlo(window);
        }

        czas_gry = (double)(clock() - start_gry) / CLOCKS_PER_SEC;

        if (!to_serwer)
        {
            ruchGracza.Wyswietlaj(window);
            wyswietlacz.Wyswietlaj(window);
        }

        //ZAKONCZENIE GRY
        int winner = rozgrywka.nr_zwyciezcy(misja_ustawienia.JedenGracz() && !rozgrywka.Graczu(nr_gracza).aktywny);
        if (winner > -1)
        {
            muzykant.Zamilcz();

            if (!to_serwer)
            {
                GUI::aplikacja().finish_viewport_render(view);
                if (interfejs != nullptr)
                    GUI::aplikacja().remove_active_window(interfejs);
            }
            auto& gwinner = rozgrywka.Graczu(winner);
            misja_ustawienia.ile_kto_wygranych[winner]++;
            printf("Zwyciezca jest: %s\n", gwinner.nazwa);
            LOG(INFO) << "Zwyciezca jest: " << gwinner.nazwa;
            if (misja_ustawienia.Zwyciezca() < 0 && !to_serwer)
                zakonczenie_gry(gwinner, nr_gracza);

            auto zwyciezca_meczu = misja_ustawienia.Zwyciezca();
            if (zwyciezca_meczu >= 0 && !to_serwer && GUI::aplikacja().pokaz_ranking)
            {
                zakonczenie_meczu(misja_ustawienia, rozgrywka);
            }
            trwa_gra = false;

            break;
        }

        GUI::aplikacja().okno.display();

        //Sleep(16); zaimplementujmy sensowne ograniczenie tutaj? inaczej zawsze jest max 50 fpsów, a gdy faktycznie gra każda klatka trwa długo (mamy 40 fps, to nas to przesuwa do 24)
    }

    if (!to_serwer)
        GUI::aplikacja().finish_viewport_render(view);

    GUI::aplikacja().reset_view();

    if(!to_serwer && interfejs != nullptr)
        GUI::aplikacja().remove_active_window(interfejs);
    return 0;
}

int misja(MisjaUstawienia& misja_ustawienia)
{
    Ruszacz ruszacz;
    return misja(misja_ustawienia, ruszacz);
}