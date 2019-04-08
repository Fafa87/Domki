#pragma once

#include <vector>
#include <list>
#include <cmath>

#include <SFML/Graphics.hpp>

#include "dane.h"

using namespace std;

template <class DstType, class SrcType>
bool IsType(const SrcType* src)
{
    return dynamic_cast<const DstType*>(src) != 0;
}

struct Rozgrywka
{
    bool walka_w_polu = true;

    list<Gracz> gracze;
    list<Domek> domki;
    list<Ludek> armie;
    int liczba_aktywnych_graczy=0;
    Gracz& Gracz(int numer);
    void ZmienLiczebnosc(Domek& domek, double nowa);
    void ZmienLiczebnosc(Ludek& ludek, double nowa);
    void TracLudki(Ludek& ludek,double ile);
    double Odleglosc(const Twor& twor1, const Twor& twor2);
    Ludek* Spotkanie(Ludek& ludek);
    Twor* Zlokalizuj(int x, int y);

    double PoliczAtakDomku(const Domek& domek, int liczba=-1);
    double PoliczObroneDomku(const Domek& domek);
    double PoliczSzybkoscDomku(const Domek& domek);
    std::tuple<int,int,int,int> SilaGracza(int nr_gracza);

    void ZniszczLudka(Ludek* ludek);
    void ZabierzTwor(const Twor* twor1);
    
    Domek* WskaznikDomek(int uid);
};