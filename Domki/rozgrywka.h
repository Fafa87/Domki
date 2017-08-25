#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

#include "dane.h"

using namespace std;

template <class DstType, class SrcType>
bool IsType(const SrcType* src)
{
	return static_cast<const DstType*>(src) != 0;
}

struct Rozgrywka
{
	vector<Gracz> gracze;
	vector<Domek> domki;
	vector<Ludek> armie;

	void ZmienLiczebnosc(Domek& domek, double nowa);
	void ZmienLiczebnosc(Ludek& ludek, double nowa);
	Twor* Zlokalizuj(int x, int y);
};