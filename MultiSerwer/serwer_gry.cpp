#include "../MultiSerwer/serwery.h"

KontekstGry* KontekstGry::obiekt;

void komunikat_serwer_gry()
{
    printf("Jestes serwerem, gracze sie podlaczyli, napisz 'start' aby zaczac gre:\n");
}

void start_serwer_gry(string zadanie_z_portem)
{
    auto tokeny = split(zadanie_z_portem, ' ');
    auto port = stoi(tokeny[tokeny.size() - 1]);
    tokeny.erase(tokeny.end() - 1);
    
    start_serwer_gry(join(tokeny, " "), port);
}

void start_serwer_gry(string zadanie, int port_gry)
{
    KontekstGry::o().serwer = new Serwer();
    auto& serwer = KontekstGry::o().serwer;
    auto& misja_ustawienia = KontekstGry::o().misja_ustawienia;

    auto adres = serwer->Postaw(port_gry);
    printf("%s\n", adres.ToString().c_str());

    auto czastki = split(zadanie, ' ');

    for (int a = 0, b = czastki[0].size(); a < b; a++)if (czastki[0][a] == '+')czastki[0][a] = ' ';
    for (int a = 0, b = czastki[1].size(); a < b; a++)if (czastki[1][a] == '+')czastki[1][a] = ' ';

    auto misja_folder = czastki[0];
    auto misja_nazwa = czastki[1];
    if (misja_nazwa.size() == 1)
    {
        LOG(INFO) << "Wybieram plansze numer: " << atoi(misja_nazwa.c_str());
        misja_nazwa = wczytaj_liste_plansz("Plansza\\" + misja_folder)[atoi(misja_nazwa.c_str())];
    }
    auto misja_sciezka = "Plansza\\" + misja_folder + "\\" + czastki[1];

    // jak podana liczba wygranych to ja zapisz
    int liczba_gier = 1;
    if (czastki.size() >= 3)
    {
        liczba_gier = atoi(czastki[2].c_str());
    }
    double predkosc = misja_ustawienia.szybkosc;
    if (czastki.size() >= 4)
    {
        predkosc = atof(czastki[3].c_str());
    }
    int liczba_graczy = misja_ustawienia.komputery.size() + 1;
    int liczba_ludzi = liczba_graczy;
    if (czastki.size() >= 5)
    {
        liczba_ludzi = atof(czastki[4].c_str());
    }

    misja_ustawienia = wczytaj_meta(misja_sciezka);
    misja_ustawienia.nazwa = misja_nazwa;
    misja_ustawienia.grupa = "Plansza\\" + misja_folder;
    misja_ustawienia.do_ilu_wygranych = liczba_gier;
    misja_ustawienia.ile_kto_wygranych = vector<int>(13);
    misja_ustawienia.szybkosc = predkosc;

    LOG(INFO) << "Stworzono gre na planszy " << misja_folder << misja_nazwa;
    LOG(INFO) << "Plansza na " << liczba_graczy << " graczy";
    LOG(INFO) << "Oczekuj na podlaczenie " << liczba_ludzi << " graczy";
    LOG(INFO) << "Gramy do  " << liczba_gier;

    misja_ustawienia.nazwy_graczow.push_back("Neutralny");
    for (int i = 0; i < liczba_ludzi; i++)
    {
        LOG(INFO) << "Laduje gracza nr: " << i+1;
        serwer->OczekujNaGracza();
        misja_ustawienia.nazwy_graczow.push_back(serwer->ludzie.back().nazwa);

        LOG(INFO) << "Zaladowalem gracza: " << serwer->ludzie.back().nazwa;
    }
}

void wykonaj_serwer_gry(string zadanie)
{
    auto& misja_ustawienia = KontekstGry::o().misja_ustawienia;
    auto& serwer = KontekstGry::o().serwer;
    if (zadanie.find("start") == 0)
    {
        MisjaUstawienia ustawienia = misja_ustawienia;

        vector<int> komputery_mapy = ustawienia.komputery;
        ustawienia.komputery.clear();
        for (int i = ustawienia.nazwy_graczow.size() - 2; i < komputery_mapy.size(); i++)
            ustawienia.komputery.push_back(komputery_mapy[i]);
        ustawienia.nr_gracza = 0;

        serwer->CzekajNaGotowosc();

        LOG(INFO) << "Wysylam informacje o starcie...";

        serwer->Start(ustawienia);

        //serwer->ludzie[0].wtyk->setBlocking(false);
        //serwer->ludzie[1].wtyk->setBlocking(false);

        SerwerowyRuszacz ruszacz(*serwer);

        LOG(INFO) << "Rozpoczynam misje...";

        misja(ustawienia, ruszacz);

        int liczba_gier = 0;
        for (auto w : ustawienia.ile_kto_wygranych)
            liczba_gier += w;

        LOG(INFO) << "Rozegrano juz " << liczba_gier << " gier\n";
        ustawienia.WypiszRanking();

        if (ustawienia.Zwyciezca() >= 0)
        {
            auto wygrany = ustawienia.Zwyciezca();
            printf("\n=========================\nCaly mecz wygral: %s\n=========================\n", ustawienia.nazwy_graczow[wygrany].c_str());

            delete serwer;
            serwer = nullptr;
        }
        else if(!serwer->CzyJestZawodnik())
        {
            ustawienia.do_ilu_wygranych = 0;
            LOG(INFO) << "Wszyscy gracze rozlaczeni, zamykam";
            delete serwer;
            serwer = nullptr;
        }
        misja_ustawienia = ustawienia;
    }
}