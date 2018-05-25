#include "rozgrywka.h"

int Twor::last_uid = 0;

Gracz & Rozgrywka::Gracz(int numer)
{
	auto it = gracze.begin();
	std::advance(it, numer);
	return *it;
}

void Rozgrywka::ZmienLiczebnosc(Domek & domek, double nowa)
{
	domek.liczebnosc = nowa;
	domek.rozmiar = 30 + domek.poziom + nowa / 25;
}

void Rozgrywka::ZmienLiczebnosc(Ludek & ludek, double nowa)
{
	ludek.liczebnosc = nowa;
	ludek.rozmiar = 20 + ludek.liczebnosc / 25;
}

double Rozgrywka::Odleglosc(const Twor& twor1, const Twor& twor2)
{
	PD polozenie_1 = twor1.polozenie;
	PD polozenie_2 = twor2.polozenie;

	PD w = (polozenie_1 - polozenie_2);
	return sqrt(w.x * w.x + w.y * w.y);
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

Domek* Rozgrywka::WskaznikDomek(int uid)
{
	for (auto &dom : domki)
	{
		if (dom.uid == uid)
		{
			return &dom;
		}
	}
	return nullptr;
}
