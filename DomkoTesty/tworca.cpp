#include "tworca.h"

Gracz & tworca::DodajGracza(Rozgrywka & gra, int nr_gracza)
{
	gra.gracze.push_back(Gracz());
	Gracz& gracz = gra.gracze.back();
	gracz.numer = nr_gracza;
	gracz.aktywny = true;
	return gracz;
}

void dodaj_gracza_gdy_brak(Rozgrywka& gra, int nr_gracza)
{
	while (gra.gracze.size() <= nr_gracza)
		tworca::DodajGracza(gra, gra.gracze.size());
}

Ludek& tworca::DodajLudka(Rozgrywka& gra, Domek& dokad, int nr_gracza, int liczba, PD pos)
{
	dodaj_gracza_gdy_brak(gra, nr_gracza);
	auto& g = gra.Gracz(nr_gracza);
	gra.armie.push_back(Ludek(dokad));
	Ludek & ludek = gra.armie.back();
	ludek.polozenie = pos;
	ludek.gracz = &g;
	ludek.wyglad = Wyglad::kLudek;
	gra.ZmienLiczebnosc(ludek, liczba);
	return ludek;
}

Domek& tworca::DodajDomek(Rozgrywka& gra, int nr_gracza, int liczba, PD pos)
{
	dodaj_gracza_gdy_brak(gra, nr_gracza);
	auto& g = gra.Gracz(nr_gracza);
	gra.domki.push_back(Domek());
	Domek& domek = gra.domki.back();
	domek.polozenie = pos;
	domek.gracz = &g;
	gra.ZmienLiczebnosc(domek, liczba);
	return domek;
}