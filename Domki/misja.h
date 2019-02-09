#pragma once
#include <SFML/Graphics.hpp>
#include <SFGUI/SFGUI.hpp>

#include "dane.h"
#include "dzialacze.h"
#include "wyswietlacze.h"
#include "muzyka.h"
#include "si.h"

#include<windows.h>
#include<fstream>

const string poziomy_trudnosci[] = { "pokojowy", "agresywny" };

struct MisjaUstawienia
{
	MisjaUstawienia();
	int Zwyciezca();
	bool JedenGracz();
	void WypiszRanking();

	string nazwa = "";
	double szybkosc = 1.5;
	string trudnosc = "podstawa";
	string skorka = "rycerze";
	bool walka_w_polu = true;

	int nr_gracza = 1;
	vector<string> nazwy_graczow;
	vector<int> komputery = { 2, 3, 4 };

	int do_ilu_wygranych = 0;
	vector<int> ile_kto_wygranych;
};

vector<string> wczytaj_liste_plansz();

MisjaUstawienia wczytaj_meta(string sciezka);

Rozgrywka zwarcie_rozgrywka(string sciezka);

int misja(MisjaUstawienia& misja_ustawienia, Ruszacz& ruszacz);

int misja(MisjaUstawienia& misja_ustawienia);
