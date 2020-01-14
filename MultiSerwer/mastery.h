#pragma once

#include <iterator>

#include "../Domki/multi.h"
#include "../Domki/multi_dzialacze.h"

#include "easylogging++.h" 

using namespace std;
using namespace multi;

void komunikat_masterserwer();
void komunikat_masterklient();

void start_masterserwer(string zadanie);
void wykonaj_masterserwer(string zadanie);

void start_masterklient(string zadanie);
void wykonaj_masterklient(string zadanie);

namespace mastery
{
    const int PORT_MASTER = 90;
    const string ADRES_MASTER = "domki.westeurope.cloudapp.azure.com";

    class MasterKlient
    {
    private:
        const string adres;
    public:
        Zawodnik gracz;
        MasterKlient(string nazwa) : gracz() {
            gracz.nazwa = nazwa;
            gracz.adres = Adres(sf::IpAddress::getLocalAddress().toString(), PORT_MASTER - 1); // TODO TMP
        }

        bool Podlacz();
    };

    class Pokoj
    {
    public:
        string nazwa;
        vector<Zawodnik*> pokojnicy;

        Pokoj(string nazwa) : nazwa(nazwa) {}
    };

    class MasterSerwer
    {
    private:
        sf::TcpListener nasluchiwacz;
        sf::SocketSelector wtykowiec;
        sf::UdpSocket rozsylacz;

    public:
        vector<Zawodnik> podpieci;
        vector<Pokoj> pokoje;
        bool stop = false;

        Adres Postaw(string adres = ADRES_MASTER);
    };

}