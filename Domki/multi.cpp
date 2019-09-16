#include "multi.h"
#include "Narzedzia/easylogging++.h"

#include <fstream>

using namespace multi;

Adres multi::Serwer::Postaw()
{
    if (nasluchiwacz.listen(PORT_TCP) != sf::Socket::Done)
        printf("listener buraka!");
    if (rozsylacz.bind(PORT_BROADCAST) != sf::Socket::Done)
        printf("rozsylacz buraka!");
    return Adres(sf::IpAddress::getLocalAddress().toString(), nasluchiwacz.getLocalPort());
}

void multi::Serwer::OczekujNaGracza()
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
        wysylaj++;
    }
    if (status != sf::Socket::Done)
    {
        printf("Serwer::OczekujNaGraczy buraka!\n");
    }
    else
    {
        auto nazwa = multi::Pobierz(*gracz.wtyk)[0];
        gracz.adres = Adres(gracz.wtyk->getRemoteAddress().toString(), gracz.wtyk->getRemotePort());
        gracz.nazwa = nazwa;
        nasluchiwacz.setBlocking(true);
        printf("polaczylem gracza %s\n", gracz.nazwa.c_str());

        ludzie.emplace_back(gracz);
    }
    printf("gracze polaczeni w liczbie %d\n", ludzie.size());
}

void multi::Serwer::Start(MisjaUstawienia ustawienia)
{
    for (int i = 0; i < ludzie.size(); i++)
    {
        ustawienia.nr_gracza = i + 1;
        std::stringstream ss;
        {
            cereal::BinaryOutputArchive archive(ss);
            archive(ustawienia);
        }

        multi::Wyslij(*ludzie[i].wtyk, ss.str());
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
    for (int i = 0; i < ludzie.size(); i++)
    {
        res |= multi::Wyslij(*ludzie[i].wtyk, ss.str());
    }
    return res;
}

vector<Rozkaz*> multi::Serwer::Odbierz()
{
    vector<Rozkaz*> res;
    if (wtykowiec.wait(sf::seconds(0.01)))
    {
        for (int i = 0; i < ludzie.size(); i++)
        {
            if (wtykowiec.isReady(*ludzie[i].wtyk))
            {
                auto data = multi::Pobierz(*ludzie[i].wtyk);
                for (auto &d : data)
                {
                    auto rest = d.substr(1);
                    // MULTI_ROZKAZ
                    std::stringstream ss(rest);
                    {
                        cereal::BinaryInputArchive dearchive(ss);
                        
                        Rozkaz * rozkaz;
                        if (d[0] == 'W')
                        {
                            rozkaz = new WymarszRozkaz(nullptr, nullptr);
                            dearchive(*(WymarszRozkaz*)rozkaz);
                        }
                        else if (d[0] == 'U')
                        {
                            rozkaz = new UlepszRozkaz(nullptr);
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
                        
                        res.push_back(rozkaz);
                    }
                }
            }
        }
    }
    
    return res;
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
        //printf("Buraka!\n");
        LOG(WARNING) << "Buraka! " << status;
    }
    else
    {
        LOG(INFO) << "Polaczony " << serwer.ToString();
        //printf("Polaczony!\n%s\n", serwer.ToString().c_str());
        multi::Wyslij(*wtyk, nazwa);
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
            //printf("Serwerek na: %s\n", nowy_adres.ToString().c_str());
            lista_serwerow.push_back(nowy_adres);
        }
    }
    odbieracz.unbind();
    return true;
}

pair<bool, MisjaUstawienia> multi::Klient::OczekujNaStart()
{
    MisjaUstawienia res;

    auto data = multi::Pobierz(*wtyk);
    if (data.size())
    {
        std::stringstream ss(data[0]);
        {
            cereal::BinaryInputArchive dearchive(ss);
            dearchive(res);
        }

        return { true, res };
    }
    return { false, res };
}


bool multi::Klient::Wyslij(vector<Rozkaz*> rozkazy)
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
    return true;
}


pair<bool, Rozgrywka> multi::Klient::Odbierz()
{
    Rozgrywka res;
    string last;
    vector<string> data;

    do 
    {
        data = multi::Pobierz(*wtyk);
        if (data.size())
            last = data[0];
    } while (data.size());

    if (last.size())
    {
        std::stringstream ss(last);
        {
            cereal::BinaryInputArchive archive(ss);
            archive(res);
        }
        return { true, res };
    }

    return { false, res };
}


vector<string> multi::Pobierz(sf::TcpSocket& wtyk)
{
    vector<string> res;
    sf::Packet pakiet;

    auto status = wtyk.receive(pakiet);
    if (status != sf::Socket::Done)
    {
        LOG(WARNING) << "Gracz::Pobierz buraka! Status " << status;
        return res;
    }

    while (pakiet.endOfPacket() == false)
    {
        string tekst;
        pakiet >> tekst;
        res.push_back(tekst);
    }
    return res;
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
    }
}

void multi::Podepnij(Rozgrywka& rozgrywka)
{
    for (auto& r : rozgrywka.domki)
    {
        r.gracz = &rozgrywka.Gracz(r.ser_gracz);
        for (auto dokad : r.ser_drogi)
        {
            r.drogi.push_back(rozgrywka.WskaznikDomek(dokad));
        }
    }

    for (auto& r : rozgrywka.armie)
    {
        r.gracz = &rozgrywka.Gracz(r.ser_gracz);
        r.cel = rozgrywka.WskaznikDomek(r.ser_cel);
    }
}


bool multi::Wyslij(sf::TcpSocket& wtyk, vector<string> dane)
{
    sf::Packet pakiet;
    for (auto& d : dane)
        pakiet << d;

    auto status = wtyk.send(pakiet);
    if (status != sf::Socket::Done)
    {
        LOG(WARNING) << "Gracz::Wyslij buraka! Status " << status;
        return false;
    }
    return true;
}

bool multi::Wyslij(sf::TcpSocket& wtyk, string dane)
{
    sf::Packet pakiet;
    pakiet << dane;
    auto status = wtyk.send(pakiet);
    if (status != sf::Socket::Done)
    {
        LOG(WARNING) << "Gracz::Wyslij buraka! Status " << status;
        return false;
    }
    return true;
}