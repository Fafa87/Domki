#pragma once
#include <SFML/Graphics.hpp>
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Window.hpp>

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

	string grupa = "Plansza";
	string nazwa = "";
	double szybkosc = 1.5;
	string trudnosc = poziomy_trudnosci[0];
	string skorka = "rycerze_hd";

	bool oszustwa = false,
        walka_w_polu = true,
        punkty_kontrolne = true;

	int nr_gracza = 1;
	vector<string> nazwy_graczow;
    vector<int> komputery = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };

	int do_ilu_wygranych = 0;
	vector<int> ile_kto_wygranych;
};

vector<string> wczytaj_liste_plansz(string folder);

vector<string> wczytaj_liste_folderow(string folder);

MisjaUstawienia wczytaj_meta(string sciezka);

Rozgrywka zwarcie_rozgrywka(string sciezka);

sf::View wysrodkowany_widok(list<Domek> &domki, int bottom_space=0);

shared_ptr<sfg::Window> interfejs_rozgrywki(shared_ptr<sfg::Window> interfejs, MisjaUstawienia &stan, Rozgrywka& rozgrywka, Wyswietlacz& wyswietlacz, Domek* wybrany, Twor* skupiony);

int misja(MisjaUstawienia& misja_ustawienia, Ruszacz& ruszacz);

int misja(MisjaUstawienia& misja_ustawienia);
