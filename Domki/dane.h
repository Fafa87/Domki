#pragma once

#include<vector>
#include<SFML\Main.hpp>
using namespace std; // pozwala pisa� vector<int> zamiast std::vector<int>

typedef sf::Vector2<double> PD; // skr�t dla typu oznaczaj�cego par� liczb

struct Gracz
{
	int numer,liczba_tworow=0;
	bool aktywny=true;
	string nazwa;
	sf::Color kolor;
};

enum Wyglad
{
	kNieznany = 0,
	kDomek = 1,
	kDomekWybrany = 2,
	kLudek = 3
};

struct Twor
{
	Twor() { uid = last_uid++; }

	static int last_uid;
	int uid; // unikalny identifikator
	PD polozenie;
	double rozmiar; // promien
	Wyglad wyglad; // liczba okre�lajaca jak powinien tw�r si� prezentowa�
	Gracz* gracz; // do kogo nale�y; neutralny te� jest graczem
	int ser_gracz;

	virtual ~Twor() {}
};

struct Ludek : Twor
{
	Ludek() {}
	Ludek(Twor& cel) : cel(&cel) {}
	Twor* cel; // do kogo (domka) stara si� dotrze�
	int ser_cel;
	int liczebnosc,tarcza=0; // powinna by� zwi�zana z jego rozmiarem
	float szybkosc_ludka = 1.0;
};

enum typ_domku
{
	kOsada = 0,
	kZamek = 1,
	kKuznia = 2
};

struct Domek : Twor
{
	double produkcja;
	double liczebnosc; // liczba aktualnie istniej�cych ludk�w
	int max_liczebnosc;
	
	typ_domku typdomku=kOsada;
	int poziom = 1;
	float szybkosc_ludkow=1.0;
	float wzmocnienie_ludkow = 1.0;
	int sila_ostrzalu = 10;

	vector<Domek*> drogi;
	vector<int> ser_drogi;
};
