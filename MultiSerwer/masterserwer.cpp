#include "../MultiSerwer/mastery.h"
#include "../Domki/ext_vector.h"

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

mastery::Serwer::Serwer()
{
    hol = make_unique<Pokoj>("Hol");
    pokoje.push_back(hol);
}


void mastery::Serwer::PrzeanalizujZapytanie(shared_ptr<multi::Zawodnik> ludek, string zapytanie)
{
    auto pokoj_ludka = gdzie_jest[ludek];
    if (zapytanie.find("/KTO?") == 0)
    {
        string lista_ludzi = "Drogi " + ludek->nazwa + " w pokoju '" + pokoj_ludka->nazwa + "' jest jeszcze " + to_string(pokoj_ludka->pokojnicy.size() - 1) + " osob:\n";

        for (auto& ludek2 : pokoj_ludka->pokojnicy) if (ludek2 != ludek)
        {
            lista_ludzi += "- " + ludek2->nazwa + "\n";
        }
        LOG(INFO) << "Informacja o ludziach do: " << ludek->nazwa;
        auto status = multi::Wyslij(*ludek->wtyk, lista_ludzi);
        ludek->ostatnio = status;
    }
    else if (zapytanie.find("/IDZ: ") == 0)
    {
        auto nazwa_pokoju = zapytanie.substr(6);
        // przepnij do nowego pokoju
        WyslijDoPokoju(pokoj_ludka, ludek->nazwa + " opuszcza pokoj.", ludek);
        PrzejdzDoPokoju(ludek, nazwa_pokoju);
        WyslijDoPokoju(gdzie_jest[ludek], ludek->nazwa + " wchodzi do pokoju.");
    }
    else if (zapytanie.find("/ODPAL: ") == 0)
    {
        auto komenda_serwera = zapytanie.substr(8);
        start_serwer_gry(komenda_serwera); // TODO umozliwij odpalanie wielu serwerow
        WyslijDoPokoju(gdzie_jest[ludek], "GRAJCIE! '" + komenda_serwera + "' na porcie" + to_string(PORT_TCP));
        wykonaj_serwer_gry("start");
        WyslijDoPokoju(gdzie_jest[ludek], "Gra skonczona...";
    }
    else 
    {
        // jak napis to wyslij go do wszystkich ludkow
        WyslijDoPokoju(pokoj_ludka, ludek->nazwa + ": " + zapytanie, ludek);
    }
}

void mastery::Serwer::WyslijDoPokoju(shared_ptr<Pokoj> pokoj, string tekst, shared_ptr<multi::Zawodnik> poza_osoba)
{
    for (auto& ludek : pokoj->pokojnicy) if (ludek != poza_osoba)
    {
        auto status = multi::Wyslij(*ludek->wtyk, tekst);
        ludek->ostatnio = status;
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
                auto osoba = make_shared<multi::Zawodnik>();
                osoba->nazwa = status_imie.second[0];
                osoba->ostatnio = sf::Socket::Done;
                osoba->wtyk = nowa_wtyczka;
                osoba->adres = multi::Adres(nowa_wtyczka->getRemoteAddress().toString(), nowa_wtyczka->getRemotePort());
                nowa_wtyczka = new sf::TcpSocket();
                wtykowiec.add(*osoba->wtyk);

                this->podpieci.emplace_back(osoba);

                LOG(INFO) << "Dodaje osobe: " << osoba->nazwa;
                gdzie_jest[osoba] = hol;
                hol->pokojnicy.push_back(osoba);
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
                if (wtykowiec.isReady(*ludek->wtyk))
                {
                    LOG(INFO) << "Ludek sie odezwal: " << ludek->nazwa;
                    auto status_dane = multi::Pobierz(*ludek->wtyk);
                    ludek->ostatnio = status_dane.first;
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
        vector<shared_ptr<multi::Zawodnik>> odlaczeni;
        for (auto& ludek : this->podpieci)
        {
            if (ludek->ostatnio == sf::Socket::Disconnected)
                odlaczeni.push_back(ludek);
        }
        for (auto ludek_ptr : odlaczeni)
            UsunZawodnika(ludek_ptr);

        Sleep(100);
    }
    LOG(INFO) << "Serwer wylaczony.";  // TODO narazie tylko wstrzymany
    this->dziala = false;
    nasluchiwacz.close();
}

void mastery::Serwer::PrzejdzDoPokoju(shared_ptr<multi::Zawodnik> ludek, string nazwa_pokoju)
{
    OpuscPokoj(ludek);
    DolaczDoPokoju(ludek, nazwa_pokoju);
}

void mastery::Serwer::DolaczDoPokoju(shared_ptr<multi::Zawodnik> ludek, string nazwa_pokoju)
{
    if (gdzie_jest[ludek] == hol)
    {
        // jesli nie ma to stworz pokoj
        shared_ptr<Pokoj> nowy_pokoj;
        for (auto& pokoj : pokoje)
            if (pokoj->nazwa == nazwa_pokoju)
            {
                nowy_pokoj = pokoj;
                break;
            }
        if (!nowy_pokoj)
        {
            LOG(INFO) << ludek->nazwa << " tworzy pokoj " << nazwa_pokoju;
            nowy_pokoj = make_shared<Pokoj>(nazwa_pokoju);
            pokoje.push_back(nowy_pokoj);
        }

        // dolacz do tego pokoju
        LOG(INFO) << ludek->nazwa << " wchodzi do pokoju " << nazwa_pokoju;
        remove_item(gdzie_jest[ludek]->pokojnicy, ludek);
        gdzie_jest[ludek] = nowy_pokoj;
        nowy_pokoj->pokojnicy.push_back(ludek);
    }
}

void mastery::Serwer::OpuscPokoj(shared_ptr<multi::Zawodnik> ludek)
{
    if (gdzie_jest[ludek] != hol)
    {
        // usun z o pokoju i mappera
        LOG(INFO) << ludek->nazwa << " wychodzi z pokoju.";
        remove_item(gdzie_jest[ludek]->pokojnicy, ludek);
        gdzie_jest[ludek] = hol;
        hol->pokojnicy.push_back(ludek);
  
    }
}

void mastery::Serwer::UsunZawodnika(shared_ptr<multi::Zawodnik> ludek)
{
    LOG(INFO) << "Opuscila nas osoba: " << ludek->nazwa;

    OpuscPokoj(ludek);
    wtykowiec.remove(*ludek->wtyk);
    remove_item(this->podpieci, ludek);
}
