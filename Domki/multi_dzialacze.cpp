#include "multi_dzialacze.h"

#include "Narzedzia/easylogging++.h"


SerwerowyRuszacz::SerwerowyRuszacz(multi::Serwer & serwer) : serwer(serwer)
{
}

void SerwerowyRuszacz::Ruszaj(double czas)
{
    // pobierz ruchy z klient�w
    auto& rozkazy = serwer.Odbierz();
    multi::Podepnij(*rozgrywka, rozkazy);
    PrzyjmijRuch(rozkazy);

    Ruszacz::Ruszaj(czas);

    this->czas_od_synchro += czas;
    if (this->czas_od_synchro > this->okres_synchronizacji)
    {
        // rozeslij stan gry do wszystkich klient�w
        auto res = serwer.Rozeslij(*rozgrywka);
        this->czas_od_synchro = 0;
        if (res == false)
        {
            LOG(WARNING) << "Wyslij stan: res=false, wychodze!";
            exit(-1);
        }
    }

    // usun graczy ktorzy sa rozlaczeni
    for (auto& g : this->serwer.ludzie) if (g.aktywny)
    {
        if (g.ostatnio == sf::Socket::Status::Disconnected)
        {
            LOG(INFO) << "Odlaczam gracza: " << g.nazwa;
            rozgrywka->PoddajGracza(rozgrywka->Graczu(g.nazwa));
            g.aktywny = false;
        }
    }
}

KlientowyRuszacz::KlientowyRuszacz(multi::Klient & klient) : klient(klient)
{
}

void KlientowyRuszacz::Ruszaj(double czas)
{
    // wy�lij swoje rozkazy
    auto wyslane = klient.Wyslij(kolejka_do_wykonania);
    if (wyslane != sf::Socket::Done)
    {
        LOG(WARNING) << "Wyslij rozkazy: wyslane=false, cicho sza!";
    }

    kolejka_do_wykonania.clear();
    Ruszacz::Ruszaj(czas);

    auto& res = klient.Odbierz();
    if (res.first == sf::Socket::Done)
    {
        // uaktualnij rozgrywk� - zast�p t� - przepisuj�c, adres ma zosta� ten sam
        *rozgrywka = res.second;
        multi::Podepnij(*rozgrywka);
    }
    else
    {
        if (res.first == sf::Socket::Disconnected)
        {
            LOG(WARNING) << "Odlaczono od serwera.";
            rozgrywka->PrzerwijGre();
        }
    }
}
