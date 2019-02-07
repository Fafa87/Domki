#pragma once

#include "rozgrywka.h"

class Ruszacz // wykonuje zaplanowane kroki symulacji
{
public:
	Ruszacz();
	void Ruszaj(double czas);

	Rozgrywka* rozgrywka;
	double szybkosc = 1;
	double szybkosc_ruchu = 200.0 / 3;
private:
	void PrzesuwajLudkow(double czas);
	void WalczLudkami(double czas);
	void Produkuj(double czas);
};
