#pragma once

#include<vector>
#include <SFML/Graphics.hpp>
#include<SFML/Main.hpp>
using namespace std; // pozwala pisa� vector<int> zamiast std::vector<int>

typedef sf::Vector2<double> PD; // skr�t dla typu oznaczaj�cego par� liczb

struct CelMisji
{
    string nazwa_celu = "Walka";

    int nr_defensora = 1, sila_plagi = 1, wymagany_zbior = 10000, do_zdobycia = 1, nr_impostera = 1, szybkosc_impostera = 10;
};

struct Gracz
{
    int numer,liczba_tworow=0;
    bool aktywny = true, //aktywny gracz posiada twory i produkuje ludkow, czyli nie jest neutralny i nadal ma co najmniej jeden tw�r
        istotny = false; //istotny gracz to taki ktory jest uwzgledniony w interfejsie rankingu
    string nazwa;
    sf::Color kolor;
};

enum class Wyglad
{
    kNieznany = 0,
    kWojownik = 1,
    kMiasto = 2,
    kFort = 3,
    kWieza = 4,
    kZbrojownia = 5,
    kStajnia = 6,
    kPole = 7
};

struct Twor
{
    Twor() { uid = last_uid++; wyglad_rodzaj = 0; }

    static int last_uid;
    int uid; // unikalny identifikator
    PD polozenie;
    double rozmiar; // promien
    Wyglad wyglad; // liczba okre�lajaca jak powinien tw�r si� prezentowa�
    int wyglad_rodzaj; // specyfika wygl�du (np. poziom domku)
    Gracz* gracz; // do kogo nale�y; neutralny te� jest graczem
    int ser_gracz;

    virtual ~Twor() {}
};

struct Ludek : Twor
{
    Ludek() {}
    Ludek(Twor& cel, Twor& skad) : cel(&cel) , skad(&skad){}
    Twor* cel, *skad; // do kogo (domka) stara si� dotrze�
    int ser_cel, ser_skad;
    int liczebnosc,tarcza=0; // powinna by� zwi�zana z jego rozmiarem
    double szybkosc_ludka = 1.0,droga=-1.0,dystans=-1.0;
};

enum class TypDomku
{
    kMiasto = 0,
    kFort = 1,
    kWieza = 2,
    kZbrojownia = 3,
    kStajnia = 4,
    kPole = 5
};

struct Domek : Twor
{
    double produkcja;
    double liczebnosc=-1; // liczba aktualnie istniej�cych ludk�w
    int max_liczebnosc=-1;
    
    TypDomku typdomku=TypDomku::kMiasto;
    int poziom = 1;
    bool przebudowa = true,
        ulepszanie = true,
        szybki_wymarsz = false,
        kontrola = true;

    vector<Domek*> drogi;
    vector<int> ser_drogi;
};
