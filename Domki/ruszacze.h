#pragma once

#include "rozgrywka.h"

class Ruszacz // wykonuje zaplanowane kroki symulacji
{
public:
	Ruszacz();
	void Ruszaj(float czas);

	Rozgrywka* rozgrywka;
	double szybkosc = 1;
	double szybkosc_ruchu = 200.0 / 3;
private:
	void PrzesuwajLudkow(float czas);
	void WalczLudkami(float czas);
	void Produkuj(float czas);
};
