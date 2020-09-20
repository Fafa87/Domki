#include "../MultiSerwer/serwery.h"


void komunikat_serwer_klient()
{
    printf("Jestes graczem, musisz sie polaczyc z serwerm. Wpisz: 'polacz' lub 'polacz <adres_ip_serwera>'\n");
}

void start_klient_gry(string zadanie)
{
    KontekstGry::o().klient = new Klient(zadanie);
    auto& klient = KontekstGry::o().klient;

    printf("Klient: %s\n", klient->nazwa.c_str());
}

void wykonaj_klient_gry(string zadanie)
{
    auto& klient = KontekstGry::o().klient;
    auto& misja_ustawienia = KontekstGry::o().misja_ustawienia;

    if (zadanie.find("polacz") == 0)
    {
        Adres adres;
        if (klient->lista_serwerow.size() == 0)
        {
            auto cel = zadanie.substr(7);
            auto ip_port = split(cel, ':');
            adres = Adres(ip_port[0], stoi(ip_port[1]));
        }
        else
        {
            adres = klient->lista_serwerow.back();
        }

        LOG(INFO) << "Lacze z " << adres.ip;
        klient->Podlacz(adres);

        // nie ma co czekac na gotowy
        //if (zadanie.find("gotowy") == 0)
        std::pair<sf::Socket::Status, MisjaUstawienia> res;
        do {
            if (!klient->WyslijGotowosc())
                break;

            printf("oczekuje na start misji... ");
            auto status_misja_ustawienia = klient->OczekujNaStart();
            auto misja_ustawienia = status_misja_ustawienia.second;
            if (status_misja_ustawienia.first != sf::Socket::Done)
                LOG(WARNING) << "Odebranie ustawien misji buraka! " << status_misja_ustawienia.first;

            printf("startuje misje %s\n", misja_ustawienia.nazwa.c_str());

            string test = "A";
            klient->wtyk->setBlocking(false);

            KlientowyRuszacz ruszacz(*klient);

            misja_ustawienia.komputery.clear();
            misja(misja_ustawienia, ruszacz);

            LOG(INFO) << "...misja skonczona";

            misja_ustawienia.WypiszRanking();

            klient->wtyk->setBlocking(true);
        } while (!(misja_ustawienia.Zwyciezca() >= 0));

        klient->Rozlacz();

        auto wygrany = misja_ustawienia.Zwyciezca();
        string wygrany_nazwa;
        if (wygrany < misja_ustawienia.nazwy_graczow.size())
            wygrany_nazwa = misja_ustawienia.nazwy_graczow[wygrany];
        else
            wygrany_nazwa = "komputer";
        printf("\n=========================\nCaly mecz wygral: %s\n=========================\n", wygrany_nazwa.c_str());
    }
}

void pobierz_serwery_gry()
{
    auto& klient = KontekstGry::o().klient;

    klient->lista_serwerow.clear();
    klient->SpiszSerwery();
}