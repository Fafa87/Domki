#pragma once

#include <iterator>

#include "../Domki/multi.h"
#include "../Domki/multi_dzialacze.h"

#include "easylogging++.h" 

using namespace std;

namespace mastery
{
    const int PORT_MASTER = 90;
    const string ADRES_MASTER = "domki.westeurope.cloudapp.azure.com";

    class Klient
    {
    private:
        const string adres;
    public:
        multi::Zawodnik gracz;
        Klient(string nazwa) : gracz() {
            gracz.nazwa = nazwa;
            gracz.adres = multi::Adres(sf::IpAddress::getLocalAddress().toString(), PORT_MASTER - 1); // TODO TMP
            gracz.wtyk = new sf::TcpSocket();
        }

        bool polaczony = false;
        void Podlacz(multi::Adres adres);
    };

    class Pokoj
    {
    public:
        string nazwa;
        vector<multi::Zawodnik*> pokojnicy;

        Pokoj(string nazwa) : nazwa(nazwa) {}
    };

    class Serwer
    {
    private:
        sf::TcpListener nasluchiwacz;
        sf::SocketSelector wtykowiec;
    public:
        vector<multi::Zawodnik> podpieci;
        vector<Pokoj> pokoje;
        bool dziala = false;

        void Postaw(int port);
    };
}

class KontekstSwiata {
public:
    mastery::Serwer* serwer = NULL;
    mastery::Klient* klient = NULL;

    static KontekstSwiata* obiekt;
    static KontekstSwiata& o() {
        if (obiekt == NULL)
            obiekt = new KontekstSwiata();
        return *obiekt;

    }
};


void komunikat_masterserwer(mastery::Serwer* serwer);
void komunikat_masterklient(mastery::Klient* klient);

void start_masterserwer(int port);
void wykonaj_masterserwer(mastery::Serwer* serwer, string zadanie);

void start_masterklient(string zadanie);
void wykonaj_masterklient(mastery::Klient* klient, string zadanie);