#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Main.hpp>

#include "dane.h"
#include "rozgrywka.h"

class Decydent // wprowadza rozkazy graczy
{
public:
	virtual void WykonajRuch() = 0;
	virtual void Przetworz() {}
};

class MyszDecydent : public Decydent
{
public:
	MyszDecydent(Rozgrywka& rozgrywka, Gracz& gracz);
	void Klik(double x, double y);

	virtual void Przetworz() {}
	void Przetworz(sf::Event zdarzenie);

	virtual void WykonajRuch();
private:
	Domek* cel;
	Domek* wybrany;
	Rozgrywka& rozgrywka;
	Gracz& gracz;
};

class Ruszacz // wykonuje zaplanowane kroki symulacji
{
public:
	Ruszacz(Rozgrywka& rozgrywka);
	void Ruszaj(float czas);

	double szybkosc = 2000.0 / 30;
	double szybkosc_ruchu = 1.0;
private:
	void PrzesuwajLudkow(float czas);
	void WalczLudkami();
	void Produkuj(float czas);

	Rozgrywka& rozgrywka;
};

class Wyswietlacz
{
public:
	Wyswietlacz(Rozgrywka& rozgrywka);
	void Wyswietlaj(sf::RenderWindow& okno);

	string Skorka;
private:
	Rozgrywka& rozgrywka;
	// TODO trzeba dodaæ tutaj czcionkê, aby siê czyta³o raz
	// trzeba to poszerzyæ o resztê obiektów, które s¹ wyœwietlane - np. sf::Text, podpis
	map<Twor*, sf::CircleShape> wyglad_tworow; 
	map<Wyglad, sf::Texture*> obrazek_tworow;
	sf::Font czcionka;
};