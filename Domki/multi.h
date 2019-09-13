#pragma once

#include <string>
#include <vector>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Network.hpp>

#include "misja.h"
#include "pakowanie.h"

using namespace std;

const int PORT_TCP = 85;
const int PORT_BROADCAST = 85;
const int PORT_RECEIVE = 84;

namespace multi
{
    struct Adres {
        Adres() {}
        Adres(string ip, int port) : ip(ip), port(port) {}

        string ip;
        int port;

        string ToString() { return ip + ":" + to_string(port); }

        bool operator==(const Adres &other) const
        {
            if (ip == other.ip)
                return port == other.port;
            return false;
        }
    };

    struct Zawodnik {
        Adres adres;
        string nazwa;
        sf::TcpSocket* wtyk;
    };

    void Podepnij(Rozgrywka& rozgrywka);
    void Podepnij(Rozgrywka& rozgrywka, vector<Rozkaz*> rozkazy);
    vector<string> Pobierz(sf::TcpSocket& wtyk);
    bool Wyslij(sf::TcpSocket& wtyk, string dane);
    bool Wyslij(sf::TcpSocket& wtyk, vector<string> dane);

    typedef Rozgrywka MRozgrywka;

    class Serwer
    {
    public:
        Adres Postaw();

        // oczekuje na pod��czenie si� gracza i podanie danych
        void OczekujNaGracza();

        // wy�lij info o starcie do graczy
        void Start(MisjaUstawienia ustawienia);
        bool Rozeslij(MRozgrywka& stan);
        vector<Rozkaz*> Odbierz();

        vector<Zawodnik> ludzie;
    private:
        sf::TcpListener nasluchiwacz;
        sf::SocketSelector wtykowiec;
        sf::UdpSocket rozsylacz;
    };

    class Klient
    {
    public:
        Klient(string nazwa);

        void Podlacz(Adres serwer);

        pair<bool, MisjaUstawienia> OczekujNaStart();
        bool Wyslij(vector<Rozkaz*> rozkazy);
        pair<bool, MRozgrywka> Odbierz();

        string nazwa;
        sf::TcpSocket* wtyk;
        sf::UdpSocket odbieracz;

        vector<Adres> lista_serwerow;
        bool SpiszSerwery();
    };
}
