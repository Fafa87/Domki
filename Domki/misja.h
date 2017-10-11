#pragma once
#include <SFML/Graphics.hpp>

#include "dane.h"
#include "dzialacze.h"
#include "wyswietlacze.h"
#include"si.h"

#include<windows.h>
#include<fstream>

const string poziomy_trudnosci[] = { "stara", "nowa" };

struct MisjaUstawienia
{
	string nazwa = "GRACZ";
	double szybkosc = 1;
	string trudnosc = "stara";

	int nr_gracza = 0; // -1 jeśli nie ma gracza, którym ruszamy
	vector<string> nazwy_graczow;
	vector<int> komputery = { 1, 2, 3 };
};

Rozgrywka zwarcie_rozgrywka(string sciezka);

int misja(MisjaUstawienia misja_ustawienia);
