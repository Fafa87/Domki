#include "multi.h"
#include "easylogging++.h"

#include <fstream>

using namespace multi;

Adres multi::Serwer::Postaw(int port_gry)
{
    int port_tcp = port_gry;
    int port_broadcast = port_gry;
    if (port_gry == -1)
    {
        port_tcp = PORT_TCP;
        port_broadcast = PORT_BROADCAST;
    }

    if (nasluchiwacz.listen(port_tcp) != sf::Socket::Done)
        printf("listener buraka!");
    if (rozsylacz.bind(port_broadcast) != sf::Socket::Done)
        printf("rozsylacz buraka!");
    return Adres(sf::IpAddress::getLocalAddress().toString(), nasluchiwacz.getLocalPort());
}

bool multi::Serwer::OczekujNaGracza(double limit_czasu)
{
    Zawodnik gracz;
    gracz.wtyk = new sf::TcpSocket();
    nasluchiwacz.setBlocking(false);
    sf::Socket::Status status;
    while ((status = nasluchiwacz.accept(*gracz.wtyk)) == sf::Socket::NotReady)
    {
        // wysyłaj swoje położenie wszędzie
        int wysylaj = 0;
        if (wysylaj % 10 == 0)
        {
            string powitanie = "czesc";
            auto status = rozsylacz.send(powitanie.c_str(), powitanie.size(), sf::IpAddress::Broadcast, PORT_RECEIVE);
            if (status != sf::Socket::Done)
                printf("rozsylacz buraka!");
        }
        Sleep(100);
        limit_czasu -= 100.0 / 1000;
        wysylaj++;
        if (limit_czasu <= 0)
        {
            nasluchiwacz.setBlocking(true);
            return false;
        }
    }
    if (status != sf::Socket::Done)
    {
        LOG(INFO) << "Serwer::OczekujNaGraczy buraka!";
    }
    else
    {
        auto status_nazwa = multi::Pobierz(*gracz.wtyk);
        auto nazwa = status_nazwa.second[0];
        gracz.adres = Adres(gracz.wtyk->getRemoteAddress().toString(), gracz.wtyk->getRemotePort());
        gracz.nazwa = nazwa;
        nasluchiwacz.setBlocking(true);

        ludzie.emplace_back(gracz);
    }
    LOG(INFO) << "Serwer::OczekujNaGraczy Gracze polaczeni w liczbie " << ludzie.size();
}

void multi::Serwer::CzekajNaGotowosc()
{
    // TODO jakis limit czasu?
    LOG(INFO) << "Rozpoczynam oczekiwanie...";
    for (int i = 0; i < ludzie.size(); i++) if (ludzie[i].aktywny)
    {
        auto status_nazwa = multi::Pobierz(*ludzie[i].wtyk);

        if (status_nazwa.first != sf::Socket::Done)
        {
            if (status_nazwa.first == sf::Socket::Disconnected)
            {
                LOG(WARNING) << "Gracz " << ludzie[i].nazwa << " sie odlaczyl.";
                ludzie[i].aktywny = false;
                continue;
            }
            LOG(WARNING) << "Serwer::CzekajNaGotowosc buraka!";
        }
        else
        {
            if (status_nazwa.second[0] != "GOGO")
                LOG(WARNING) << "Serwer::CzekajNaGotowosc wiadomosc" << status_nazwa.second[0];
            LOG(INFO) << "Odhaczylem " << ludzie[i].nazwa;
        }
    }
}

void multi::Serwer::Start(MisjaUstawienia ustawienia)
{
    for (int i = 0; i < ludzie.size(); i++) if (ludzie[i].aktywny)
    {
        ustawienia.nr_gracza = i + 1;
        ludzie[i].gracz_w_rozgrywce = i + 1;
        std::stringstream ss;
        {
            cereal::BinaryOutputArchive archive(ss);
            archive(ustawienia);
        }

        LOG(INFO) << "Wysylam do " << ludzie[i].nazwa;
        auto status = multi::Wyslij(*ludzie[i].wtyk, ss.str());
        if (status != sf::Socket::Done)
        {
            LOG(WARNING) << "Wyslanie startu buraka! " << status;
        }
        // Czy tutaj nie dodajemy wiele razy?
        wtykowiec.add(*ludzie[i].wtyk);
    }
}

bool multi::Serwer::Rozeslij(Rozgrywka& stan)
{
    std::stringstream ss;
    {
        cereal::BinaryOutputArchive archive(ss);
        archive(stan);
    }

    bool res = true;
    for (int i = 0; i < ludzie.size(); i++)  if (ludzie[i].aktywny)
    {
        auto stan_wyslania = multi::Wyslij(*ludzie[i].wtyk, ss.str());
        ludzie[i].ostatnio = stan_wyslania;
        res |= stan_wyslania == sf::Socket::Status::Done;
    }
    return res;
}

vector<Rozkaz*> multi::Serwer::Odbierz()
{
    vector<Rozkaz*> res;
    if (wtykowiec.wait(sf::seconds(0.01)))
    {
        for (int i = 0; i < ludzie.size(); i++) if (ludzie[i].aktywny)
        {
            if (wtykowiec.isReady(*ludzie[i].wtyk))
            {
                auto status_data = multi::Pobierz(*ludzie[i].wtyk);
                ludzie[i].ostatnio = status_data.first;
                for (auto &d : status_data.second)
                {
                    auto rest = d.substr(1);
                    // MULTI_ROZKAZ
                    std::stringstream ss(rest);
                    {
                        cereal::BinaryInputArchive dearchive(ss);
                        
                        Rozkaz * rozkaz = nullptr;
                        if (d[0] == 'W')
                        {
                            rozkaz = new WymarszRozkaz();
                            dearchive(*(WymarszRozkaz*)rozkaz);
                        }
                        else if (d[0] == 'U')
                        {
                            rozkaz = new UlepszRozkaz();
                            dearchive(*(UlepszRozkaz*)rozkaz);
                        }
                        else if (d[0] == 'P')
                        {
                            rozkaz = new PrzebudujRozkaz(nullptr, TypDomku::kMiasto);
                            dearchive(*(PrzebudujRozkaz*)rozkaz);
                        }
                        else if (d[0] == 'B')
                        {
                            rozkaz = new BurzRozkaz(nullptr);
                            dearchive(*(BurzRozkaz*)rozkaz);
                        }
                        
                        if (rozkaz != nullptr)
                        {
                            rozkaz->kto_nr = ludzie[i].gracz_w_rozgrywce;
                            res.push_back(rozkaz);
                        }
                        else
                        {
                            LOG(WARNING) << "Czekam na rozkaz a dostalem: " << d;
                        }
                    }
                }
            }
        }
    }
    
    return res;
}

bool multi::Serwer::CzyJestZawodnik()
{
    for (auto& zaw : this->ludzie)
        if (zaw.aktywny)
            return true;
    return false;
}

multi::Klient::Klient(string nazwa)
{
    this->nazwa = nazwa;
}

void multi::Klient::Podlacz(Adres serwer)
{
    wtyk = new sf::TcpSocket();
    sf::Socket::Status status = wtyk->connect(serwer.ip, serwer.port);
    if (status != sf::Socket::Done)
    {
        LOG(WARNING) << "Podlaczenie buraka! " << status;
    }
    else
    {
        LOG(INFO) << "Polaczony " << serwer.ToString();
        status = multi::Wyslij(*wtyk, nazwa);
        if (status != sf::Socket::Done)
            LOG(WARNING) << "Wyslanie nazwy do serwera buraka! " << status;
    }
}

void multi::Klient::Rozlacz()
{
    wtyk->disconnect();
    LOG(INFO) << "Odlaczony od serwera";
}

bool multi::Klient::WyslijGotowosc()
{
    auto status = multi::Wyslij(*this->wtyk, "GOGO");
    if (status == sf::Socket::Done)
    {
        return true;
    }
    else if (status == sf::Socket::Disconnected)
    {
        LOG(WARNING) << "Klient::WyslijGotowosc rozlaczone!";
        return false;
    }
    else 
    {
        LOG(WARNING) << "Klient::WyslijGotowosc buraka!";
        return false;
    }
}

bool multi::Klient::SpiszSerwery()
{
    char data[100];
    sf::IpAddress serwer;
    unsigned short port;
    std::size_t received;
    odbieracz.bind(PORT_RECEIVE);
    Sleep(1000);
    odbieracz.setBlocking(false);
    auto status = odbieracz.receive(data, 100, received, serwer, port);
    data[received] = 0;
    if (status != sf::Socket::Done) 
    {
        LOG(WARNING) << "Klient::SpiszSerwery buraka! Status " << status;
        odbieracz.unbind();
        return status != sf::Socket::Error;
    }
    else if (string(data, received) == "czesc")
    {
        Adres nowy_adres(serwer.toString(), (int)port);
        if (count(lista_serwerow.begin(), lista_serwerow.end(), nowy_adres) == 0)
        {
            LOG(INFO) << "Serwerek na: " << nowy_adres.ToString();
            lista_serwerow.push_back(nowy_adres);
        }
    }
    odbieracz.unbind();
    return true;
}

pair<sf::Socket::Status, MisjaUstawienia> multi::Klient::OczekujNaStart()
{
    MisjaUstawienia res;

    LOG(INFO) << "Klient::OczekujNaStart";
    auto status_data = multi::Pobierz(*wtyk);
    LOG(INFO) << "Klient::OczekujNaStart->status:" << status_data.first;
    auto data = status_data.second;
    if (data.size())
    {
        std::stringstream ss(data[0]);
        {
            cereal::BinaryInputArchive dearchive(ss);
            dearchive(res);
        }
    }
    return { status_data.first, res };
}


sf::Socket::Status multi::Klient::Wyslij(vector<Rozkaz*> rozkazy)
{
    vector<string> dane;
    for (auto r : rozkazy)
    {
        std::stringstream ss;
        {
            cereal::BinaryOutputArchive archive(ss);
            // MULTI ROZKAZ
            if (IsType<WymarszRozkaz>(r))
            {
                ss << "W";
                WymarszRozkaz* rozkaz = (WymarszRozkaz*)r;
                archive(*rozkaz);
            }
            else if (IsType<UlepszRozkaz>(r))
            {
                ss << "U";
                UlepszRozkaz* rozkaz = (UlepszRozkaz*)r;
                archive(*rozkaz);
            }
            else if (IsType<PrzebudujRozkaz>(r))
            {
                ss << "P";
                PrzebudujRozkaz* rozkaz = (PrzebudujRozkaz*)r;
                archive(*rozkaz);
            }
            else if (IsType<BurzRozkaz>(r))
            {
                ss << "B";
                BurzRozkaz* rozkaz = (BurzRozkaz*)r;
                archive(*rozkaz);
            }
        }
        printf("%s\n", ss.str().c_str());
        dane.push_back(ss.str());
    }

    if (dane.size() > 0)
        return multi::Wyslij(*wtyk, dane);
    return sf::Socket::Status::Done;
}


pair<sf::Socket::Status, Rozgrywka> multi::Klient::Odbierz()
{
    Rozgrywka res;
    string last;
    pair<sf::Socket::Status, vector<string>> status_data;

    do 
    {
        status_data = multi::Pobierz(*wtyk);
        if (status_data.second.size())
            last = status_data.second[0];
    } while (status_data.second.size());

    if (last.size())
    {
        std::stringstream ss(last);
        {
            cereal::BinaryInputArchive archive(ss);
            archive(res);
        }
        return { sf::Socket::Status::Done, res };
    }

    return { status_data.first, res };
}


pair<sf::Socket::Status, vector<string>> multi::Pobierz(sf::TcpSocket& wtyk)
{
    vector<string> res;
    sf::Packet pakiet;

    auto status = wtyk.receive(pakiet);
    if (status != sf::Socket::Done)
    {
        if (status != sf::Socket::NotReady)
            LOG_EVERY_N(10, WARNING) << "Gracz::Pobierz buraka! Status " << status;
        return { status, res };
    }

    while (pakiet.endOfPacket() == false)
    {
        string tekst;
        pakiet >> tekst;
        res.push_back(tekst);
    }
    return { status, res };
}


pair<sf::Socket::Status, vector<string>> multi::Pobierz(sf::TcpSocket& wtyk, sf::Time limit_czasu)
{
    vector<string> res;
    sf::SocketSelector selector;
    selector.add(wtyk);
    if (selector.wait(limit_czasu))
    {
        return Pobierz(wtyk);
    }
    else
        return { sf::Socket::NotReady, res };
}


void multi::Podepnij(Rozgrywka& rozgrywka, vector<Rozkaz*> rozkazy)
{
    for (auto r : rozkazy)
    {
        // MULTI ROZKAZ
        if (IsType<WymarszRozkaz>(r))
        {
            WymarszRozkaz * rozkaz = (WymarszRozkaz*)r;
            rozkaz->dokad = rozgrywka.WskaznikDomek(rozkaz->ser_dokad);
            rozkaz->skad = rozgrywka.WskaznikDomek(rozkaz->ser_skad);
        }
        else if (IsType<UlepszRozkaz>(r))
        {
            UlepszRozkaz * rozkaz = (UlepszRozkaz*)r;
            rozkaz->kogo = rozgrywka.WskaznikDomek(rozkaz->ser_kogo);
        }
        else if (IsType<PrzebudujRozkaz>(r))
        {
            PrzebudujRozkaz * rozkaz = (PrzebudujRozkaz*)r;
            rozkaz->kogo = rozgrywka.WskaznikDomek(rozkaz->ser_kogo);
        }
        else if (IsType<BurzRozkaz>(r))
        {
            BurzRozkaz * rozkaz = (BurzRozkaz*)r;
            rozkaz->kogo = rozgrywka.WskaznikDomek(rozkaz->ser_kogo);
        }

        r->kto_wydal_rozkaz = &rozgrywka.Graczu(r->kto_nr);
    }
}

void multi::Podepnij(Rozgrywka& rozgrywka)
{
    for (auto& r : rozgrywka.domki)
    {
        r.gracz = &rozgrywka.Graczu(r.ser_gracz);
        for (auto dokad : r.ser_drogi)
        {
            r.drogi.push_back(rozgrywka.WskaznikDomek(dokad));
        }
    }

    for (auto& r : rozgrywka.armie)
    {
        r.gracz = &rozgrywka.Graczu(r.ser_gracz);
        r.cel = rozgrywka.WskaznikDomek(r.ser_cel);
    }
}


sf::Socket::Status multi::Wyslij(sf::TcpSocket& wtyk, vector<string> dane)
{
    sf::Packet pakiet;
    for (auto& d : dane)
        pakiet << d;

    auto status = wtyk.send(pakiet);
    if (status != sf::Socket::Done)
    {
        LOG_EVERY_N(10, WARNING) << "Gracz::Wyslij buraka! Status " << status;
        return status;
    }
    return status;
}

sf::Socket::Status multi::Wyslij(sf::TcpSocket& wtyk, string dane)
{
    sf::Packet pakiet;
    pakiet << dane;
    auto status = wtyk.send(pakiet);
    if (status != sf::Socket::Done)
    {
        LOG_EVERY_N(10, WARNING) << "Gracz::Wyslij buraka! Status " << status;
        return status;
    }
    return status;
}