#include "../MultiSerwer/mastery.h"
#include "../Domki/ext_vector.h"

#include <thread>
#include "../Domki/razem.h"

KontekstSwiata* KontekstSwiata::obiekt;

void komunikat_masterserwer(mastery::Serwer* serwer)
{
    if (serwer->dziala)
        printf("Jestes postawionym multiserwerem. Napisz stop aby umrzec:'\n");
    else
        printf("Jestes spiacym multiserwerem. Napisz postaw <port> aby sie obudzic.'\n");
}

void start_masterserwer(int port, int porty_gry_od, int porty_gry_do)
{
    KontekstSwiata::o().serwer = new mastery::Serwer(porty_gry_od, porty_gry_do);
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
    this->porty_gier = make_pair(85, 100);
    hol = make_shared<Pokoj>("hol");
    pokoje.push_back(hol);
}

mastery::Serwer::Serwer(int porty_gry_od, int porty_gry_do) : mastery::Serwer()
{
    this->porty_gier = make_pair(porty_gry_od, porty_gry_do);
}

void mastery::Serwer::PrzeanalizujZapytanie(shared_ptr<multi::Zawodnik> ludek, string zapytanie)
{
    auto pokoj_ludka = gdzie_jest[ludek];
    if (zapytanie.find("/KTO?") == 0)
    {
        string lista_ludzi = "Drogi " + ludek->nazwa + " w pokoju '" + pokoj_ludka->nazwa + "' jest " + to_string(pokoj_ludka->pokojnicy.size()) + " osob:\n";

        for (auto& ludek2 : pokoj_ludka->pokojnicy)
        {
            lista_ludzi += ludek2->nazwa + "\n";
        }
        LOG(INFO) << "Informacja o ludziach do: " << ludek->nazwa;
        auto status = multi::Wyslij(*ludek->wtyk, lista_ludzi);
        ludek->ostatnio = status;
    }
    else if (zapytanie.find("/SWIAT?") == 0)
    {
        string caly_swiat = "SWIAT:\n";
        for (auto& pokoj : this->pokoje)
        {
            string ten_pokoj = pokoj->nazwa + ":";
            for (auto& ludek : pokoj->pokojnicy)
            {
                ten_pokoj += ludek->nazwa + ",";
            }
            caly_swiat += ten_pokoj + "\n";
        }
        LOG(INFO) << "Informacja o swiecie do: " << ludek->nazwa;
        auto status = multi::Wyslij(*ludek->wtyk, caly_swiat);
        ludek->ostatnio = status;
    }
    else if (zapytanie.find("/IDZ: ") == 0)
    {
        auto nazwa_pokoju = zapytanie.substr(6);
        // przepnij do nowego pokoju
        if (PrzejdzDoPokoju(ludek, nazwa_pokoju))
        {
            WyslijDoPokoju(pokoj_ludka, ludek->nazwa + " opuszcza pokoj.", ludek);
            auto status = multi::Wyslij(*ludek->wtyk, "Wszedles do pokoju " + nazwa_pokoju);
            ludek->ostatnio = status;
            WyslijDoPokoju(gdzie_jest[ludek], ludek->nazwa + " wchodzi do pokoju.", ludek);
            if (gdzie_jest[ludek]->aktywne_info.size())
                multi::Wyslij(*ludek->wtyk, gdzie_jest[ludek]->aktywne_info); // wy�lij informacje o serwerze
        }
    }
    else if (zapytanie.find("/START: ") == 0)
    {
        auto wolny_port = ZnajdzWolnyPort();
        if (wolny_port == -1)
        {
            WyslijDoPokoju(gdzie_jest[ludek], "Brak wolnych miejsc na serwerze. Trzeba czekac...");
        }
        else
        {
            auto komenda_serwera = zapytanie.substr(8);
            auto info_dla_graczy = "GRAJCIE! '" + komenda_serwera + "' na porcie " + to_string(wolny_port);
            WyslijDoPokoju(gdzie_jest[ludek], info_dla_graczy);
            
            // wpisz process
            auto proces_gry = start_nowej_gry_dla_wielu("0 " + komenda_serwera + " " + to_string(wolny_port));
            gdzie_jest[ludek]->aktywny_port = wolny_port;
            gdzie_jest[ludek]->aktywna_gra = proces_gry;
            gdzie_jest[ludek]->aktywne_info = info_dla_graczy;
        }
    }
    else 
    {
        // jak napis to wyslij go do wszystkich ludkow
        WyslijDoPokoju(pokoj_ludka, ludek->nazwa + ": " + zapytanie);
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

string mastery::Serwer::WyznaczStatystyki()
{
    std::ostringstream statystyki;
    statystyki << "Witamy Ciebie na planecie Domkow!" << std::endl;
    statystyki << "W sumie jest tutaj " << podpieci.size() << " osob." << std::endl;
    statystyki << "Istnieje " << pokoje.size() << " pokoi ";
    statystyki << "i trwa wlasnie " << std::count_if(pokoje.begin(), pokoje.end(), [](auto pokoj) {return pokoj->aktywny_port != -1; }) << " rozgrywek." << std::endl;
    statystyki << "Maksymalna liczba rozgrywek naraz to: " << porty_gier.second - porty_gier.first << ".";
    return statystyki.str();
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
            auto status_imie = multi::Pobierz(*nowa_wtyczka, sf::seconds(10));
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

                this->podpieci.push_back(osoba);

                multi::Wyslij(*osoba->wtyk, this->WyznaczStatystyki());

                LOG(INFO) << "Dodaje osobe: " << osoba->nazwa;
                DolaczDoPokoju(osoba, hol->nazwa);
                WyslijDoPokoju(hol, osoba->nazwa + " wchodzi do pokoju.", osoba);
                if (gdzie_jest[osoba]->aktywne_info.size())
                    multi::Wyslij(*osoba->wtyk, gdzie_jest[osoba]->aktywne_info); // wy�lij informacje o serwerze
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

        // sprawdz czy rozgrywki sie nie skonczyly
        for (auto pokoj : this->pokoje)
        {
            if (pokoj->aktywny_port != -1)
            {
                DWORD exit_code;
                GetExitCodeProcess(pokoj->aktywna_gra.hProcess, &exit_code);
                if (exit_code != STILL_ACTIVE)
                {
                    // usun rozgrywke
                    pokoj->aktywna_gra = PROCESS_INFORMATION();
                    pokoj->aktywny_port = -1;
                    pokoj->aktywne_info = "";
                    WyslijDoPokoju(pokoj, "Gra skonczona...");
                }
            }
        }

        Sleep(100);
    }
    LOG(INFO) << "Serwer wylaczony.";  // TODO narazie tylko wstrzymany
    this->dziala = false;
    nasluchiwacz.close();
}

int mastery::Serwer::ZnajdzWolnyPort()
{
    set<int> wolne_porty;
    for (int i = porty_gier.first; i <= porty_gier.second; i++)
        wolne_porty.insert(i);

    // nie umrzyj prosze
    for (auto pokoj : pokoje)
    {
        if (pokoj->aktywny_port != -1)
        {
            wolne_porty.erase(pokoj->aktywny_port);
        }
    }
    if (wolne_porty.size() == 0)
        return -1;
    return *wolne_porty.begin();
}

bool mastery::Serwer::PrzejdzDoPokoju(shared_ptr<multi::Zawodnik> ludek, string nazwa_pokoju)
{
    if (gdzie_jest[ludek]->nazwa != nazwa_pokoju)
    {
        OpuscPokoj(ludek);
        DolaczDoPokoju(ludek, nazwa_pokoju);
        return true;
    }
    return false;
}

void mastery::Serwer::DolaczDoPokoju(shared_ptr<multi::Zawodnik> ludek, string nazwa_pokoju)
{
    if (gdzie_jest.count(ludek) == 0)
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
        nowy_pokoj->pokojnicy.push_back(ludek); 
        gdzie_jest[ludek] = nowy_pokoj;
    }
}

void mastery::Serwer::OpuscPokoj(shared_ptr<multi::Zawodnik> ludek)
{
    if (gdzie_jest.count(ludek) == 1)
    {
        auto pokoj = gdzie_jest[ludek];
        // usun z o pokoju i mappera
        LOG(INFO) << ludek->nazwa << " wychodzi z pokoju.";
        remove_item(gdzie_jest[ludek]->pokojnicy, ludek);
        gdzie_jest.erase(ludek);

        // usun pusty pokoj jesli nie jest holem
        if (pokoj->pokojnicy.size() == 0 && this->hol != pokoj)
        {
            LOG(INFO) << "Usuwam pokoj " << pokoj->nazwa;
            remove_item(pokoje, pokoj);
        }
    }
}

void mastery::Serwer::UsunZawodnika(shared_ptr<multi::Zawodnik> ludek)
{
    LOG(INFO) << "Opuscila nas osoba: " << ludek->nazwa;

    WyslijDoPokoju(gdzie_jest[ludek], ludek->nazwa + " opuszcza pokoj.", ludek);
    OpuscPokoj(ludek);
    WyslijDoPokoju(hol, ludek->nazwa + " uciekl.");

    wtykowiec.remove(*ludek->wtyk);
    remove_item(this->podpieci, ludek);
}
