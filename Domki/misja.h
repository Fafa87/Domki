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
	string nazwa;
	double szybkosc;
	string trudnosc;
};

Rozgrywka zwarcie_rozgrywka(string sciezka);

int misja(MisjaUstawienia misja_ustawienia);
