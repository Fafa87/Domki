#pragma once

#include <iterator>

#include "../Domki/multi.h"
#include "../Domki/multi_dzialacze.h"

#include "../MultiSerwer/serwery.h"

#include "easylogging++.h" 
#include "BlockingCollection.h"

using namespace std;

namespace mastery
{
    const int PORT_MASTER = 90;
    const string ADRES_MASTER = "domki.westeurope.cloudapp.azure.com";

    class Klient
    {
    private:
        const string adres;

        bool oczekuje_na_liste = false;
        vector<string> ludzie_obok;
    public:
        multi::Zawodnik gracz;
        code_machina::BlockingCollection<string> komendy;
        code_machina::BlockingCollection<string> odebrane;

        Klient(string nazwa) : gracz() {
            gracz.nazwa = nazwa;
            gracz.adres = multi::Adres(sf::IpAddress::getLocalAddress().toString(), PORT_MASTER - 1); // TODO TMP
            gracz.wtyk = new sf::TcpSocket();
        }

        bool polaczony = false;
        Adres adres_serwer;

        void Podlacz(multi::Adres adres);
        void Rozlacz();
        void KtoJest();
        void KtoNaSwiecie();
        void IdzDo(string pokoj);

        void PrzeanalizujOdebrane(string odebrane);
        vector<string> KtoJestObok();
        vector<string> KtoJestNaPlanecie();
        bool RozgrywkaTrwa();
        Adres rozgrywka_pokoju;
    };

    class Pokoj
    {
    public:
        string nazwa;
        vector<shared_ptr<multi::Zawodnik>> pokojnicy;

        int aktywny_port=-1;
        PROCESS_INFORMATION aktywna_gra;
        string aktywne_info;

        Pokoj(string nazwa) : nazwa(nazwa) {}
    };

    class Serwer
    {
    private:
        sf::TcpListener nasluchiwacz;
        sf::SocketSelector wtykowiec;
        pair<int, int> porty_gier;

    public:
        Serwer();
        Serwer(int porty_gry_od, int porty_gry_do);

        vector<shared_ptr<multi::Zawodnik>> podpieci;
        map<shared_ptr<multi::Zawodnik>, shared_ptr<Pokoj>> gdzie_jest;
        shared_ptr<Pokoj> hol;
        vector<shared_ptr<Pokoj>> pokoje;

        bool dziala = false;
        string WyznaczStatystyki();
        
        void Postaw(int port);
        int ZnajdzWolnyPort();

        void PrzeanalizujZapytanie(shared_ptr<multi::Zawodnik> ludek, string zapytanie);
        void WyslijDoPokoju(shared_ptr<Pokoj> pokoj, string tekst, shared_ptr<multi::Zawodnik> poza_osoba = nullptr);

        void WyslijNaSwiat(string tekst);
        void SpisLudnosci();

        bool PrzejdzDoPokoju(shared_ptr<multi::Zawodnik> ludek, string nazwa_pokoju);
        void DolaczDoPokoju(shared_ptr<multi::Zawodnik> ludek, string nazwa_pokoju);
        void OpuscPokoj(shared_ptr<multi::Zawodnik> ludek);

        void UsunZawodnika(shared_ptr<multi::Zawodnik> ludek);
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

void start_masterserwer(int port, int porty_gry_od, int porty_gry_do);
void wykonaj_masterserwer(mastery::Serwer* serwer, string zadanie);

void start_masterklient(string zadanie);
void wykonaj_masterklient(mastery::Klient* klient, string zadanie);