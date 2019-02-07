#include "dzialacze.h"
#include"si.h"
#include <set>
#include <string>
#include<ctime>
#include<cmath>

Komputer::Komputer(Rozgrywka & rozgrywka, Gracz & gracz,double szybkosc_komputera) : rozgrywka(rozgrywka), gracz(gracz),szybkosc_komputera(szybkosc_komputera)
{
	czas = (double)(rand() % 30)/10.0;
}

double licz_przyrost(double odleglosc)
{
	double przyrostarmii = 2.0;
	double przesuniecie = 200.0 / 3.0;
	return przyrostarmii*odleglosc / przesuniecie;
}

vector<Rozkaz*> Komputer::WykonajRuch()
{
	vector<Rozkaz*> res;
	if (czas*szybkosc_komputera >= 3.0)
	{
		czas -= 3.0;
		bool ruch,graniczy;
		double odleglosc = 0;
		Domek* domekx = NULL;
		for (Domek& domek1 : rozgrywka.domki)
			if (domek1.gracz->numer == gracz.numer)
			{
				graniczy = false;
				for (Domek* domek2 : domek1.drogi)
					{
					if (domek2->gracz->numer != gracz.numer &&
						((domek2->gracz->aktywny&& 2* (domek2->liczebnosc + 5) <= domek1.liczebnosc)|| domek2->gracz->aktywny==false&&2*domek2->liczebnosc<=domek1.liczebnosc))
						{
						auto r = new WymarszRozkaz(&domek1, domek2);
						r->ulamek = 1.0;
						res.push_back(r);
						graniczy = true;
						}
					}
				if (!graniczy)
					{
					if (2 * domek1.liczebnosc >= domek1.max_liczebnosc&&domek1.poziom <= 2)
					{
						UlepszRozkaz* r = new UlepszRozkaz(&domek1);
						res.push_back(r);
						ruch = true;
					}
					else
					{
						ruch = false;
						graniczy = false;
						for (Domek* domek2 : domek1.drogi)
						{
							if (domek2->gracz->numer != gracz.numer)graniczy = true;
						}

					}
					if (!ruch&&!graniczy)
					{
						for (Domek& romek1 : rozgrywka.domki)
						{
							if (2 * romek1.liczebnosc >= romek1.max_liczebnosc&&romek1.poziom <= 2);
							else
							{
								auto r = new WymarszRozkaz(&domek1, &romek1);
								r->ulamek = 1.0;
								res.push_back(r);
								ruch = true;
								break;
							}
						}
						
					}
					}	
			}
	}
	return res;
}

KomputerSilver::KomputerSilver(Rozgrywka & rozgrywka, Gracz & gracz, double szybkosc_komputera) : Komputer(rozgrywka, gracz,szybkosc_komputera)
{


}

vector<Rozkaz*> KomputerSilver::WykonajRuch()
{
	vector<Rozkaz*> res;
	return res;
}