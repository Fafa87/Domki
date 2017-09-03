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

class Komputer : public Decydent
{
public:
	Komputer(Rozgrywka& rozgrywka,Gracz& gracz);
	Rozgrywka& rozgrywka;
	Gracz& gracz;
	float czas=0.0,
		szybkosc_komputera=3.0;
	virtual void WykonajRuch();
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
	vector<int> klikniecia;

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

	double szybkosc = 1;
	double szybkosc_ruchu = 200.0/3;
private:
	void PrzesuwajLudkow(float czas);
	void WalczLudkami(float czas);
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
	// TODO trzeba to poszerzy� o reszt� obiekt�w, kt�re s� wy�wietlane - np. sf::Text, podpis
	map<Twor*, sf::CircleShape> wyglad_tworow; 
	map<Wyglad, sf::Texture*> obrazek_tworow;
	sf::Font czcionka;
};