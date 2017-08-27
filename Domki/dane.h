#pragma once

#include<vector>
#include<SFML\Main.hpp>
using namespace std; // pozwala pisa� vector<int> zamiast std::vector<int>

typedef sf::Vector2<double> PD; // skr�t dla typu oznaczaj�cego par� liczb

struct Gracz
{
	int numer;
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
	PD polozenie;
	double rozmiar; // promien
	Wyglad wyglad; // liczba okre�lajaca jak powinien tw�r si� prezentowa�
	Gracz* gracz; // do kogo nale�y; neutralny te� jest graczem

	virtual ~Twor() {}
};

struct Ludek : Twor
{
	Ludek(Twor& cel) : cel(cel) {}
	Twor& cel; // do kogo (domka) stara si� dotrze�
	int liczebnosc; // powinna by� zwi�zana z jego rozmiarem
};

struct Domek : Twor
{
	double produkcja;
	double liczebnosc; // liczba aktualnie istniej�cych ludk�w
	int max_liczebnosc;
};
