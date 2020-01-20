#include "../MultiSerwer/mastery.h"


void komunikat_masterklient(mastery::Klient* klient)
{
    printf("Jestes zwiedzajacym. Mozesz sie podlaczyc do masterserwera, a potem z nim konwersowac.\n");
    if (klient->polaczony)
        printf("Jestes juz polaczony, mozesz napisac tekst lub zapytac serwer o to /KTO? jest w pokoju serwera.'\n");
    else
        printf("Nie jestes jeszcze polaczony do serwera. Napisz polacz <adres>, <port>, aby sprobowac sie polaczyc do msa.");
}

void start_masterklient(string nazwa)
{
    KontekstSwiata::o().klient = new mastery::Klient(nazwa);
    auto& klient = KontekstSwiata::o().klient;
}

void wykonaj_masterklient(mastery::Klient* klient, string zadanie)
{



    // TODO do usuniecia potem (gdy bedzie masterserwer)
    //if (zadanie.find("odbierz") == 0)
    //{
    //    vector<vector<string>> wiad;
    //    /*if (serwer != NULL)
    //    wiad = serwer->Odbierz();*/
    //    if (klient != NULL)
    //        wiad.push_back(Pobierz(*klient->wtyk).second);
    //    printf("Odebralem:\n");
    //    for (auto l : wiad)
    //    {
    //        for (auto s : l)
    //            printf("'%s'\n", s.c_str());
    //        printf("----\n");
    //    }
    //}
    //if (zadanie.find("napisz") == 0)
    //{
    //    auto text = zadanie.substr(7);
    //    /*if (serwer != NULL)
    //        serwer->Rozeslij(text);
    //    if (klient != NULL)
    //        Wyslij(*klient->wtyk, text);*/
    //}

    //auto& klient = Kontekst::o().klient;
    //auto& misja_ustawienia = Kontekst::o().misja_ustawienia;

    //if (zadanie.find("polacz") == 0)
    //{
    //    Adres adres;
    //    if (klient->lista_serwerow.size() == 0)
    //    {
    //        auto cel = zadanie.substr(7);
    //        auto ip_port = split(cel, ':');
    //        adres = Adres(ip_port[0], stoi(ip_port[1]));
    //    }
    //    else
    //    {
    //        adres = klient->lista_serwerow.back();
    //    }
    //    klient->Podlacz(adres);

    //    // nie ma co czekac na gotowy
    //    //if (zadanie.find("gotowy") == 0)
    //    std::pair<sf::Socket::Status, MisjaUstawienia> res;
    //    do {
    //        printf("oczekuje na start misji... ");
    //        auto status_misja_ustawienia = klient->OczekujNaStart();
    //        auto misja_ustawienia = status_misja_ustawienia.second;
    //        if (status_misja_ustawienia.first != sf::Socket::Done)
    //            LOG(WARNING) << "Odebranie ustawien misji buraka! " << status_misja_ustawienia.first;

    //        printf("startuje misje %s\n", misja_ustawienia.nazwa.c_str());

    //        string test = "A";
    //        klient->wtyk->setBlocking(false);

    //        KlientowyRuszacz ruszacz(*klient);

    //        misja_ustawienia.komputery.clear();
    //        misja(misja_ustawienia, ruszacz);

    //        printf("...misja skonczona\n");

    //        misja_ustawienia.WypiszRanking();

    //        klient->wtyk->setBlocking(true);
    //    } while (!(misja_ustawienia.Zwyciezca() >= 0));

    //    auto wygrany = misja_ustawienia.Zwyciezca();
    //    printf("\n=========================\nCaly mecz wygral: %s\n=========================\n", misja_ustawienia.nazwy_graczow[wygrany].c_str());
    //}
}