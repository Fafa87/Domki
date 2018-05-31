#pragma once
#include <SFML/Graphics.hpp>

#include "dane.h"
#include "dzialacze.h"
#include "wyswietlacze.h"
#include"si.h"

#include<windows.h>
#include<fstream>

const string poziomy_trudnosci[] = { "podstawa", "rozszerzenie" };

struct MisjaUstawienia
{
	string nazwa = "";
	double szybkosc = 1;
	string trudnosc = "podstawa";
	string skorka = "rycerze";
	bool walka_w_polu = true;

	int nr_gracza = 1;
	vector<string> nazwy_graczow;
	vector<int> komputery = { 2, 3, 4 };
};

Rozgrywka zwarcie_rozgrywka(string sciezka);

int misja(MisjaUstawienia misja_ustawienia, Ruszacz& ruszacz);

int misja(MisjaUstawienia misja_ustawienia);
