#pragma once

#include "../Domki/dane.h"
#include "../Domki/rozgrywka.h"

namespace tworca
{
	Gracz& DodajGracza(Rozgrywka& gra, int nr_gracza);

	Ludek& DodajLudka(Rozgrywka& gra, Domek& dokad, int nr_gracza, int liczba, PD pos);
	Domek& DodajDomek(Rozgrywka& gra, int nr_gracza, int liczba, PD pos);

}