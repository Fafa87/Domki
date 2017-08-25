#include "rozgrywka.h"

void Rozgrywka::ZmienLiczebnosc(Domek & domek, double nowa)
{
	domek.liczebnosc = nowa;
	domek.rozmiar = 20 + nowa / 10;
}

void Rozgrywka::ZmienLiczebnosc(Ludek & ludek, double nowa)
{
	ludek.liczebnosc = nowa;
	ludek.rozmiar = 2 + ludek.liczebnosc / 2;
}

Twor * Rozgrywka::Zlokalizuj(int x, int y)
{
	for (auto& dom : domki)
	{
		PD punkt(x, y);
		PD roz = punkt - dom.polozenie;
		if (sqrt(roz.x * roz.x + roz.y * roz.y) < dom.rozmiar)
			return &dom;
	}
	return nullptr;
}
