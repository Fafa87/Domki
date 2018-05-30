#pragma once

#include<vector>
#include<SFML\Main.hpp>
using namespace std; // pozwala pisaæ vector<int> zamiast std::vector<int>

typedef sf::Vector2<double> PD; // skrót dla typu oznaczaj¹cego parê liczb

struct Gracz
{
	int numer,liczba_tworow=0;
	bool aktywny=true;
	string nazwa;
	sf::Color kolor;
};

enum class Wyglad
{
	kNieznany = 0,
	kDomek = 1,
	kDomekWybrany = 2,
	kLudek = 3,
	kObrona = 4,
	kUlepszacz = 5
};

struct Twor
{
	Twor() { uid = last_uid++; }

	static int last_uid;
	int uid; // unikalny identifikator
	PD polozenie;
	double rozmiar; // promien
	Wyglad wyglad; // liczba okreœlajaca jak powinien twór siê prezentowaæ
	Gracz* gracz; // do kogo nale¿y; neutralny te¿ jest graczem
	int ser_gracz;

	virtual ~Twor() {}
};

struct Ludek : Twor
{
	Ludek() {}
	Ludek(Twor& cel) : cel(&cel) {}
	Twor* cel; // do kogo (domka) stara siê dotrzeæ
	int ser_cel;
	int liczebnosc,tarcza=0; // powinna byæ zwi¹zana z jego rozmiarem
	float szybkosc_ludka = 1.0;
};

enum class TypDomku
{
	kOsada = 0,
	kZamek = 1,
	kKuznia = 2
};

struct Domek : Twor
{
	double produkcja;
	double liczebnosc; // liczba aktualnie istniej¹cych ludków
	int max_liczebnosc;
	
	TypDomku typdomku=TypDomku::kOsada;
	int poziom = 1;
	float szybkosc_ludkow=1.0;
	float wzmocnienie_ludkow = 1.0;
	int sila_ostrzalu = 10;

	vector<Domek*> drogi;
	vector<int> ser_drogi;
};
