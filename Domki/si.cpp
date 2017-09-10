#include "dzialacze.h"
#include"si.h"
#include <set>
#include <string>
#include<ctime>
#include<cmath>

Komputer::Komputer(Rozgrywka & rozgrywka, Gracz & gracz) : rozgrywka(rozgrywka), gracz(gracz)
{
}

double licz_przyrost(double odleglosc)
{
	double przyrostarmii = 2.0;
	double przesuniecie = 200.0 / 3.0;
	return przyrostarmii*odleglosc / przesuniecie;
}

void Komputer::WykonajRuch()
{
	if (czas >= 3.0)
	{
		czas -= 3.0;
		bool ruch = false;
		float odleglosc = 0;
		Domek* domekx = NULL;
		for (Domek& domek1 : rozgrywka.domki)
			if (!ruch&&domek1.gracz->numer == gracz.numer)
			{
				ruch = false;
				for (Domek& domek2 : rozgrywka.domki)
				{
					if (domek2.gracz->numer != gracz.numer && ((!domek2.gracz->aktywny && domek2.liczebnosc * 2 <= domek1.liczebnosc) || (domek2.gracz->aktywny&&domek2.gracz->numer != gracz.numer && (domek1.liczebnosc >= (domek2.liczebnosc + licz_przyrost(rozgrywka.Odleglosc(domek1, domek2))) * 2 + 1)) && (odleglosc == 0 || rozgrywka.Odleglosc(domek1, domek2) < odleglosc)))
					{
						if (!ruch)ruch = true;
						odleglosc = rozgrywka.Odleglosc(domek1, domek2);
						domekx = &domek2;
					}
				}
				if (ruch)
				{
					Domek* wybrany = &domek1;
					Domek* cel = domekx;
					auto liczba = int(wybrany->liczebnosc / 2);
					if (liczba > 0 && cel != wybrany)
					{
						rozgrywka.ZmienLiczebnosc(*wybrany, wybrany->liczebnosc - liczba);
						rozgrywka.armie.push_back(Ludek(*cel));
						Ludek& nowaArmia = rozgrywka.armie.back();
						nowaArmia.gracz = &gracz;
						nowaArmia.polozenie = wybrany->polozenie;
						nowaArmia.wyglad = Wyglad::kLudek;
						rozgrywka.ZmienLiczebnosc(nowaArmia, liczba);
						domek1.gracz->liczba_tworow++;
					}
				}
			}
	}
}

KomputerSilver::KomputerSilver(Rozgrywka & rozgrywka, Gracz & gracz) : Komputer(rozgrywka, gracz)
{
}

void KomputerSilver::WykonajRuch()
{
	
}