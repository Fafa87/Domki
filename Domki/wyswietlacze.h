#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Main.hpp>

#include "os.h"
#include "dane.h"
#include "rozgrywka.h"
#include "dzialacze.h"
#include "Narzedzia\Animation.hpp"

class Wyswietlacz
{
public:
	Wyswietlacz(Rozgrywka& rozgrywka);
	Animation ZaladujAnimacje(string & sciezka);
	void Wyswietlaj(sf::RenderWindow& okno);
	void WyswietlTlo(sf::RenderWindow& okno);

	void Zaladuj(string wybrana_skora);
	void ZaladujInne();

private:
	Rozgrywka& rozgrywka;
	map<Twor*, sf::RectangleShape> wyglad_tworow;
	map<Wyglad, Animation> obrazek_tworow;

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