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
	domek.liczebnosc = std::min((double)domek.max_liczebnosc, nowa);
	domek.rozmiar = 10 + 10 * domek.poziom + nowa / 10;
}

void Rozgrywka::ZmienLiczebnosc(Ludek & ludek, double nowa)
{
	ludek.liczebnosc = nowa;
	ludek.rozmiar = 10 + ludek.liczebnosc / 2;
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
		for (auto& armia : armie) if (&armia != &ludek)
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
