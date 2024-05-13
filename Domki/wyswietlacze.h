#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Main.hpp>

#include "os.h"
#include "dane.h"
#include "rozgrywka.h"
#include "dzialacze.h"
#include "Narzedzia\Animation.hpp"

class ZestawAnimacji {
public:
    vector<Animation> wszystkieAnimacje;

    static map<string, sf::Texture*> ZaladowaneObrazy;
    static sf::Texture* ZaladujIZapamietaj(string &sciezka);
    static Animation ZaladujAnimacje(string & sciezka);
    static ZestawAnimacji ZaladujZPliku(string & sciezka_szablon);

    Animation PobierzAnimacjePoziomu(int poziom);

    sf::IntRect Rozmiar();
};



class Wyswietlacz
{
public:
    Wyswietlacz(Rozgrywka& rozgrywka);
    void Wyswietlaj(sf::RenderWindow& okno);
    void WyswietlTlo(sf::RenderWindow& okno, sf::View& widok);

    sf::Image StworzMinimape(PD wielkosc);
    sf::View ProstyWidokMapy();

    void UaktualnijWyglad(Twor*);

    void Zaladuj(string wybrana_skora);
    void ZaladujInne();
    sf::RectangleShape PobierzWyglad(Twor* domek);

private:
    Rozgrywka& rozgrywka;
    map<Twor*, sf::RectangleShape> wyglad_tworow;
    map<Wyglad, ZestawAnimacji> obrazek_tworow;

    sf::Texture obrazek_tla;
    sf::Font czcionka;

    string skorka;
};

class OznaczaczWyborow
{
public:
    OznaczaczWyborow(MyszDecydent& decydent);
    void Wyswietlaj(sf::RenderWindow& okno);

    Domek* WybranyDomek();
private:
    MyszDecydent& decydent;
    float cykl_czasowy();
    void rysuj_strzalke(sf::RenderWindow & okno, sf::Vector2f start, sf::Vector2f kierunek, sf::Color color, float grubosc);
};