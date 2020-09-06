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


void mastery::Serwer::PrzeanalizujZapytanie(multi::Zawodnik& ludek, string zapytanie)
{
    if (zapytanie.find("/KTO?") == 0)
    {
        // TODO wysylaj ile jest
    }
    else {
        // jak napis to wyslij go do wszystkich ludkow
        for (auto& ludek2 : this->podpieci) if (&ludek2 != &ludek)
        {
            LOG(INFO) << "Przesylam to do: " << ludek2.nazwa;
            auto status = multi::Wyslij(*ludek2.wtyk, ludek2.nazwa + ": " + zapytanie);
            ludek2.ostatnio = status;
        }
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

        if (wtykowiec.wait(sf::seconds(0.1)))
        {
            for (auto& ludek : this->podpieci)
            {
                if (wtykowiec.isReady(*ludek.wtyk))
                {
                    LOG(INFO) << "Ludek sie odezwal: " << ludek.nazwa;
                    auto status_dane = multi::Pobierz(*ludek.wtyk);
                    ludek.ostatnio = status_dane.first;
                    // TODO zareaguj gdyby sie okazalo ze jednak jest rozlaczone
                    if (status_dane.first == sf::Socket::Done)
                    {
                        PrzeanalizujZapytanie(ludek, status_dane.second[0]);
                    }
                    else
                    {
                        LOG(INFO) << "Nie udalo sie odebrac - status: " << status_dane.first;
                    }
                }
            }
        }

        // usun odlaczonych gosci
        this->podpieci.erase(std::remove_if(this->podpieci.begin(), this->podpieci.end(),
            [](multi::Zawodnik& z) {
                if (z.ostatnio == sf::Socket::Disconnected)
                {
                    LOG(INFO) << "Opuscila nas osoba: " << z.nazwa;
                    return true;
                }
                return false;
            }
        ), this->podpieci.end());

        Sleep(100);
    }
    LOG(INFO) << "Serwer wylaczony.";  // TODO narazie tylko wstrzymany
    this->dziala = false;
    nasluchiwacz.close();
}