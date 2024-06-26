#pragma once

#include "../Domki/gui.h"
#include "../Domki/misja.h"
#include "../Domki/os.h"

#include "Thor/Shapes/Arrow.hpp"

#include <iterator>

class DecydentEdytor {
public:
    DecydentEdytor(sf::RenderWindow& okno, Rozgrywka& rozgrywka);
    void resetuj();

    Twor* wybrany;
    Domek tworzony;

    Domek* pierwszy, *drugi;
    sf::Vector2f miejsce_tworzenia;


    bool tworz,
        laczenie_miast;

    std::pair<bool, bool> otwieraj, zapisuj;

    int  ladowanie_ludkow;


    void Przetworz(sf::Event zdarzenie);
    void Wykonaj();

    void rysuj_strzalke(sf::View widok);

private:
    sf::RenderWindow & okno;
    Rozgrywka& rozgrywka;
};