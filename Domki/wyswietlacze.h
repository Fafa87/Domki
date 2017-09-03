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

	string Skorka;
private:
	Rozgrywka& rozgrywka;
	map<Twor*, sf::CircleShape> wyglad_tworow;
	map<Wyglad, sf::Texture*> obrazek_tworow;
	sf::Font czcionka;
};

class OznaczaczWyborow
{
public:
	OznaczaczWyborow(MyszDecydent& decydent);
	void Wyswietlaj(sf::RenderWindow& okno);
private:
	MyszDecydent& decydent;
};