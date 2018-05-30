#include "rozgrywka.h"

int Twor::last_uid = 0;

Gracz & Rozgrywka::Gracz(int numer)
{
	auto it = gracze.begin();
	std::advance(it, numer);
	return *it;
}

void Rozgrywka::ZniszczLudka(Ludek* ludek)
{
	auto it = armie.begin();
	for (; it != armie.end(); it++)
	{
		if (&(*it) == ludek)
			break;
	}

	if (it != armie.end())
	{
		ZabierzTwor(ludek);
		armie.erase(it);
	}
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

void Rozgrywka::TracLudki(Ludek & ludek, double ile)
{
	ludek.tarcza = std::max(0,ludek.tarcza - (int)ile);
	ile -= (double)ludek.tarcza;
	if (ile > 0)ludek.liczebnosc = std::max(0.0,ludek.liczebnosc-ile);
			
}

void Rozgrywka::ZabierzTwor(const Twor* twor)
{
	twor->gracz->liczba_tworow--;
	if (twor->gracz->liczba_tworow == 0)
	{
		if (twor->gracz->aktywny) liczba_aktywnych_graczy--;
		twor->gracz->aktywny = false;
	}
}

double Rozgrywka::Odleglosc(const Twor& twor1, const Twor& twor2)
{
	PD polozenie_1 = twor1.polozenie;
	PD polozenie_2 = twor2.polozenie;

	PD w = (polozenie_1 - polozenie_2);
	return sqrt(w.x * w.x + w.y * w.y);
}

Ludek * Rozgrywka::Spotkanie(Ludek & ludek)
{
	if (walka_w_polu)
	{
		for (auto& armia : armie) if (&armia != &ludek &&  armia.gracz != ludek.gracz)
		{
			double odl = Odleglosc(ludek, armia);
			if (odl < (armia.rozmiar + ludek.rozmiar) / 2)
				return &armia;
		}
	}
	return nullptr;
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
