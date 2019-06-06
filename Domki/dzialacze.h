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

struct Testpower : Rozkaz
{
	Testpower(Domek* kogo);

	Domek * kogo;
	int ser_kogo;
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

struct PrzebudujRozkaz : Rozkaz
{
	PrzebudujRozkaz(Domek* kogo, TypDomku naco);

	Domek * kogo;
	TypDomku naco;
	int ser_kogo;
};

struct BurzRozkaz : Rozkaz
{
	BurzRozkaz(Domek* kogo);

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
	char nacisniety = 0;
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
	virtual void Ruszaj(double czas);
	void PrzyjmijRuch(vector<Rozkaz*> rozkazy);

	Rozgrywka* rozgrywka;
	double szybkosc = 1;
	double szybkosc_ruchu = 200.0/3;

	virtual ~Ruszacz() {}

	int ile_armii_idzie = 0;
	int armie_ktore_dotarly = 0;
protected:
	vector<Rozkaz*> kolejka_do_wykonania;
private:
	void WykonajRuchy();
	void PrzesuwajLudkow(double czas);
	void WalczLudkami(double czas);
	void Produkuj(double czas);
	void Strzelaj(double czas);
};
