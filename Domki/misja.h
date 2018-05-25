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
	string nazwa = "";
	double szybkosc = 1;
	string trudnosc = "stara";
	bool walka_w_polu = true;

	int nr_gracza = 0;
	vector<string> nazwy_graczow;
	vector<int> komputery = { 1, 2, 3 };
};

Rozgrywka zwarcie_rozgrywka(string sciezka);

int misja(MisjaUstawienia misja_ustawienia, Ruszacz& ruszacz);

int misja(MisjaUstawienia misja_ustawienia);
