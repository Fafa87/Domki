#pragma once

#include"dzialacze.h"

class Komputer : public Decydent
{
public:
	Komputer(Rozgrywka& rozgrywka, Gracz& gracz,double szybkosc_komputera,char styl);
	Rozgrywka& rozgrywka;
	Gracz& gracz;
	double czas = 0.0,
		szybkosc_komputera=1.0;
	char styl='P';// 'P' - oszust 'S' - budowniczy 'A' - agresor 'Z' - kawalerzysta
	virtual vector<Rozkaz*> WykonajRuch();
};

class KomputerSilver : public Komputer
{
public:
	KomputerSilver(Rozgrywka& rozgrywka, Gracz& gracz,double szybkosc_komputera,char styl);
	virtual vector<Rozkaz*> WykonajRuch();
};