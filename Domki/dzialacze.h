#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Main.hpp>

#include "dane.h"
#include "rozgrywka.h"

struct Rozkaz
{
	// to string to cereal serialization?
    Gracz &kto_wydal_rozkaz;
    Rozkaz(Gracz &kto_wydal_rozkaz) : kto_wydal_rozkaz(kto_wydal_rozkaz){}
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
    WymarszRozkaz(Domek* skad, Domek* dokad, Gracz& kto_wydal_rozkaz);
	Domek * skad, * dokad;
	int ser_skad, ser_dokad;
	double ulamek = 1;
};

struct UlepszRozkaz : Rozkaz
{
    UlepszRozkaz(Domek* kogo);
	UlepszRozkaz(Domek* kogo, Gracz& kto_wydal_rozkaz);

	Domek * kogo;
	int ser_kogo;
};

struct PrzebudujRozkaz : Rozkaz
{
    PrzebudujRozkaz(Domek* kogo, TypDomku naco);
	PrzebudujRozkaz(Domek* kogo, TypDomku naco, Gracz& kto_wydal_rozkaz);

	Domek * kogo;
	TypDomku naco;
	int ser_kogo;
};

struct BurzRozkaz : Rozkaz
{
    BurzRozkaz(Domek* kogo);
	BurzRozkaz(Domek* kogo, Gracz& kto_wydal_rozkaz);

	Domek * kogo;
	int ser_kogo;
};

class Decydent // wprowadza rozkazy graczy
{
public:
	virtual vector<Rozkaz*> WykonajRuch() = 0;
};

class MyszDecydent : public Decydent
{
public:
	MyszDecydent(sf::RenderWindow& okno, Rozgrywka& rozgrywka, Gracz& gracz);
	void Klik(double x, double y);

	virtual void Przetworz() {}
	void Przetworz(sf::Event zdarzenie);
    void Potworz(sf::Event zdarzenie);

	virtual vector<Rozkaz*> WykonajRuch();

    bool kontrola = false;
    map<Domek*, Domek*> punkty_kontrolne;
	Domek* wybrany = nullptr, *kontrolowany = nullptr;
	Twor* skupiony = nullptr;
	char nacisniety = 0;
	Gracz& gracz;

private:
	sf::RenderWindow & okno;
	vector<pair<int, sf::Mouse::Button>> klikniecia;
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
	void Strzelaj();
};
