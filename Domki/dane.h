#pragma once

#include<vector>
#include <SFML/Graphics.hpp>
#include<SFML/Main.hpp>
using namespace std; // pozwala pisaæ vector<int> zamiast std::vector<int>

typedef sf::Vector2<double> PD; // skrót dla typu oznaczaj¹cego parê liczb

struct Gracz
{
	int numer,liczba_tworow=0;
	bool aktywny=true, istotny=false;
	string nazwa;
	sf::Color kolor;
};

enum class Wyglad
{
	kNieznany = 0,
	kWojownik = 1,
	kMiasto = 2,
	kFort = 3,
	kWieza = 4,
	kZbrojownia = 5,
	kStajnia = 6,
	kPole = 7
};

struct Twor
{
	Twor() { uid = last_uid++; wyglad_rodzaj = 0; }

	static int last_uid;
	int uid; // unikalny identifikator
	PD polozenie;
	double rozmiar; // promien
	Wyglad wyglad; // liczba okreœlajaca jak powinien twór siê prezentowaæ
	int wyglad_rodzaj; // specyfika wygl¹du (np. poziom domku)
	Gracz* gracz; // do kogo nale¿y; neutralny te¿ jest graczem
	int ser_gracz;

	virtual ~Twor() {}
};

struct Ludek : Twor
{
	Ludek() {}
	Ludek(Twor& cel, Twor& skad) : cel(&cel) , skad(&skad){}
	Twor* cel, *skad; // do kogo (domka) stara siê dotrzeæ
	int ser_cel;
	int liczebnosc,tarcza=0; // powinna byæ zwi¹zana z jego rozmiarem
	double szybkosc_ludka = 1.0,droga=-1.0,dystans=-1.0;
};

enum class TypDomku
{
	kMiasto = 0,
	kFort = 1,
	kWieza = 2,
	kZbrojownia = 3,
	kStajnia = 4,
	kPole = 5
};

struct Domek : Twor
{
	double produkcja;
	double liczebnosc=-1; // liczba aktualnie istniej¹cych ludków
	int max_liczebnosc=-1;
	
	TypDomku typdomku=TypDomku::kMiasto;
	int poziom = 1;
    bool przebudowa = true,
        ulepszanie = true,
        szybki_wymarsz = false;

    Twor* szybki_przemarsz = nullptr;

	vector<Domek*> drogi;
	vector<int> ser_drogi;
};
