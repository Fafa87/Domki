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
	MyszDecydent(sf::RenderWindow& okno, Rozgrywka& rozgrywka, Gracz& gracz);
	void Klik(double x, double y);

	virtual void Przetworz() {}
	void Przetworz(sf::Event zdarzenie);

	virtual void WykonajRuch();

	Domek* wybrany;
	Gracz& gracz;

private:
	sf::RenderWindow & okno;
	vector<int> klikniecia;
	Domek* cel;
	Rozgrywka& rozgrywka;
};

class Ruszacz // wykonuje zaplanowane kroki symulacji
{
public:
	Ruszacz(Rozgrywka& rozgrywka);
	void Ruszaj(float czas);

	double szybkosc = 1;
	double szybkosc_ruchu = 200.0/3;
private:
	void PrzesuwajLudkow(float czas);
	void WalczLudkami(float czas);
	void Produkuj(float czas);

	Rozgrywka& rozgrywka;
};
