#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Main.hpp>

#include "dane.h"
#include "rozgrywka.h"
#include "dzialacze.h"

class Wyswietlacz
{
public:
	Wyswietlacz(Rozgrywka& rozgrywka);
	void Wyswietlaj(sf::RenderWindow& okno);
	void WyswietlTlo(sf::RenderWindow& okno);

	void Zaladuj(string wybrana_skora);

private:
	Rozgrywka& rozgrywka;
	map<Twor*, sf::RectangleShape> wyglad_tworow;
	map<Wyglad, sf::Texture*> obrazek_tworow;

	sf::Texture obrazek_tla;
	sf::Font czcionka;

	string skorka;
};

class OznaczaczWyborow
{
public:
	OznaczaczWyborow(MyszDecydent& decydent);
	void Wyswietlaj(sf::RenderWindow& okno);
private:
	MyszDecydent& decydent;
};