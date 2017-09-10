#pragma once

#include"dzialacze.h"

class Komputer : public Decydent
{
public:
	Komputer(Rozgrywka& rozgrywka, Gracz& gracz);
	Rozgrywka& rozgrywka;
	Gracz& gracz;
	float czas = 0.0,
		szybkosc_komputera = 3.0;
	virtual void WykonajRuch();
};

class KomputerSilver : public Komputer
{
	virtual void WykonajRuch();
};