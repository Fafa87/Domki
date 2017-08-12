#pragma once

#include <SFML/Graphics.hpp>

#include "dane.h"

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
	Domek* wybrany;
	Rozgrywka& rozgrywka;
	Gracz& gracz;
};

class Ruszacz // wykonuje zaplanowane kroki symulacji
{
public:
	Ruszacz(Rozgrywka& rozgrywka);
	void Ruszaj();

	double szybkosc = 1.0 / 30;
private:
	void PrzesuwajLudkow();
	void WalczLudkami();
	void Produkuj();

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
	map<Twor*, sf::CircleShape> wyglad_tworow;
};