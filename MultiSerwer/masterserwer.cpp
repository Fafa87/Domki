#include "../MultiSerwer/mastery.h"

#include <thread>

KontekstSwiata* KontekstSwiata::obiekt;

void komunikat_masterserwer(mastery::Serwer* serwer)
{
    if (serwer->dziala)
        printf("Jestes postawionym multiserwerem. Napisz stop aby umrzec:'\n");
    else
        printf("Jestes spiacym multiserwerem. Napisz postaw <port> aby sie obudzic.'\n");
}

void start_masterserwer(int port)
{
    KontekstSwiata::o().serwer = new mastery::Serwer();
    auto& serwer = KontekstSwiata::o().serwer;
    std::thread([&serwer, port]() { serwer->Postaw(port); }).detach();
}

void wykonaj_masterserwer(mastery::Serwer* serwer, string zadanie)
{
    if (zadanie.find("stop") == 0 && serwer->dziala == true)
    {
        LOG(INFO) << "Zatrzymuje serwer...";
        serwer->dziala = false;
    }
    if (zadanie.find("postaw") == 0 && serwer->dziala == false)
    {
        auto port = stoi(zadanie.substr(7));
        std::thread([serwer, port]() { serwer->Postaw(port); }).detach();
    }
}

void mastery::Serwer::Postaw(int port)
{
    if (nasluchiwacz.listen(port) != sf::Socket::Done)
    {
        LOG(ERROR) << "Nie udalo sie podpiac pod port.";
        return;
    }
    nasluchiwacz.setBlocking(false);

    LOG(INFO) << "Serwer stoi na adresie: " << sf::IpAddress::getLocalAddress().toString() << ":" << port;
    this->dziala = true;

    auto nowa_wtyczka = new sf::TcpSocket(); // TODO to powinno byc kiedys usuwane z pamieci
    while (this->dziala)
    {
        // sprawdz czy nikt nie chce sie dostac
        auto status = nasluchiwacz.accept(*nowa_wtyczka);
        if (status == sf::Socket::Done)
        {
            LOG(INFO) << "Kobylka u plota...";

            // wczytaj jego imie
            nowa_wtyczka->setBlocking(true);
            auto status_imie = multi::Pobierz(*nowa_wtyczka);
            if (status_imie.first == sf::Socket::Done && status_imie.second.size() == 1)
            {
                // utworz go i dodaj do listy
                multi::Zawodnik osoba;
                osoba.nazwa = status_imie.second[0];
                osoba.ostatnio = sf::Socket::Done;
                osoba.wtyk = nowa_wtyczka;
                osoba.adres = multi::Adres(nowa_wtyczka->getRemoteAddress().toString(), nowa_wtyczka->getRemotePort());
                nowa_wtyczka = new sf::TcpSocket();
                wtykowiec.add(*osoba.wtyk);
                this->podpieci.emplace_back(osoba);

                LOG(INFO) << "Dodaje osobe: " << osoba.nazwa;
            }
            else 
            {
                LOG(INFO) << "Przeganiam, bo nie znam: " << status_imie.first;
                nowa_wtyczka->disconnect();
            }
        }

        // sprawdz czy nikt nic nie pisze

        Sleep(100);
    }
    LOG(INFO) << "Serwer wylaczony.";
    this->dziala = false;
}