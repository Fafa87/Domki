#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Main.hpp>

#include "dane.h"
#include "rozgrywka.h"

struct Rozkaz
{
	// to string to cereal serialization?
	virtual ~Rozkaz() {}
};

struct WymarszRozkaz : Rozkaz
{
	WymarszRozkaz(Domek* skad, Domek* dokad);

	Domek * skad, * dokad;
	int ser_skad, ser_dokad;
	double ulamek = 1;
};

struct UlepszRozkaz : Rozkaz
{
	UlepszRozkaz(Domek* kogo);

	Domek * kogo;
	int ser_kogo;
};

class Decydent // wprowadza rozkazy graczy
{
public:
	virtual vector<Rozkaz*> WykonajRuch() = 0;
	virtual void Przetworz() {}
};

class MyszDecydent : public Decydent
{
public:
	MyszDecydent(sf::RenderWindow& okno, Rozgrywka& rozgrywka, Gracz& gracz);
	void Klik(double x, double y);

	virtual void Przetworz() {}
	void Przetworz(sf::Event zdarzenie);

	virtual vector<Rozkaz*> WykonajRuch();

	Domek* wybrany = nullptr;
	Gracz& gracz;

private:
	sf::RenderWindow & okno;
	vector<int> klikniecia;
	Domek* cel = nullptr;
	Rozgrywka& rozgrywka;
};

class Ruszacz // wykonuje zaplanowane kroki symulacji
{
public:
	Ruszacz();
	virtual void Ruszaj(float czas);
	void PrzyjmijRuch(vector<Rozkaz*> rozkazy);

	Rozgrywka* rozgrywka;
	double szybkosc = 1;
	double szybkosc_ruchu = 200.0/3;

	virtual ~Ruszacz() {}

protected:
	vector<Rozkaz*> kolejka_do_wykonania;

private:
	void WykonajRuchy();
	void PrzesuwajLudkow(float czas);
	void WalczLudkami(float czas);
	void Produkuj(float czas);
	void Strzelaj(float czas);
};
