#include "../MultiSerwer/serwery.h"


void komunikat_masterklient()
{
    //printf("Jestes graczem, musisz sie polaczyc z serwerm. Wpisz: 'polacz' lub 'polacz <adres_ip_serwera>'\n");
}

void start_masterklient_gry(string zadanie)
{
    /*Kontekst::o().klient = new Klient(zadanie.substr(7));
    auto& klient = Kontekst::o().klient;

    printf("Klient: %s\n", klient->nazwa.c_str());*/
}

void wykonaj_masterklient_gry(string zadanie)
{
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