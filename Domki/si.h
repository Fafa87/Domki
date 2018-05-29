#pragma once

#include"dzialacze.h"

class Komputer : public Decydent
{
public:
	Komputer(Rozgrywka& rozgrywka, Gracz& gracz,float szybkosc_komputera);
	Rozgrywka& rozgrywka;
	Gracz& gracz;
	float czas = 0.0,
		szybkosc_komputera=1.0;
	virtual vector<Rozkaz*> WykonajRuch();
};

class KomputerSilver : public Komputer
{
public:
	KomputerSilver(Rozgrywka& rozgrywka, Gracz& gracz,float szybkosc_komputera);
	virtual vector<Rozkaz*> WykonajRuch();
};