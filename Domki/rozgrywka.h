#pragma once

#include <vector>
#include <cmath>

#include <SFML/Graphics.hpp>

#include "dane.h"

using namespace std;

template <class DstType, class SrcType>
bool IsType(const SrcType* src)
{
	return dynamic_cast<const DstType*>(src) != 0;
}

struct Rozgrywka
{
	vector<Gracz> gracze;
	vector<Domek> domki;
	vector<Ludek> armie;

	void ZmienLiczebnosc(Domek& domek, double nowa);
	void ZmienLiczebnosc(Ludek& ludek, double nowa);
	double Odleglosc(const Twor& twor1, const Twor& twor2);
	Twor* Zlokalizuj(int x, int y);
};