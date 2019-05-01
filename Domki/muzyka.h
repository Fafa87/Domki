#pragma once

#include <SFML/Audio.hpp>

#include "os.h"
#include "rozgrywka.h"
#include "dzialacze.h"

class Muzykant
{
public:
    Muzykant(Rozgrywka& rozgrywka);
    void Zaladuj(string wybrana_skora);
    void Przygrywaj();
    void NoGraj();
    void Zamilcz();
    void GrajEfekty(Ruszacz& ruchy);

    bool wyciszony = false;
private:
    Rozgrywka& rozgrywka;

    sf::SoundBuffer marszBuffer;
    sf::Sound marsz;

    sf::Music przygrywka;
    sf::Music przygrywkaOkrzyki;

    sf::SoundBuffer zdobycieBuffer;
    sf::Sound zdobycie;

    string skorka;
};
