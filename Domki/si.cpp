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

double ile_trzeba_ludkow(double odleglosc,Domek* domek)
{
	double przyrostarmii = 2.0;
	double przesuniecie = 200.0 / 3.0;
	if (domek->typdomku == TypDomku::kMiasto&&domek->gracz->aktywny==true)return domek->liczebnosc+przyrostarmii * domek->poziom * odleglosc / przesuniecie + 1.0;
	else if (domek->typdomku == TypDomku::kFort)return domek->liczebnosc*(domek->poziom+1.0)+1.0;
	else if (domek->typdomku == TypDomku::kWieza)return domek->liczebnosc + 1.0 + 100*domek->poziom*domek->poziom;
	return domek->liczebnosc + 1.0;
}

vector<Rozkaz*> Komputer::WykonajRuch()
{
	vector<Rozkaz*> res;
	if (czas*szybkosc_komputera >= 1.0)
	{
		czas -= 1.0;
		bool ruch, graniczy;
		double odleglosc = 0;
		Domek* domekx = NULL; 
		//rozgrywka.SilaGracza(gracz.numer); ilosc poziomow kuzni (float)(std::get<2>(rozgrywka.SilaGracza(gracz.numer)))
		for (Domek& domek1 : rozgrywka.domki)
			if (domek1.gracz->numer == gracz.numer&&10*domek1.liczebnosc>=domek1.max_liczebnosc)
			{
				graniczy = false;
				for (Domek* domek2 : domek1.drogi)
				{
					if(domek2->gracz->numer!=gracz.numer&&2*ile_trzeba_ludkow(rozgrywka.Odleglosc(domek1,(*domek2)),domek2)+1.0<=domek1.liczebnosc*(10.0+ (float)(std::get<2>(rozgrywka.SilaGracza(gracz.numer))))/10.0)
					{
						auto r = new WymarszRozkaz(&domek1, domek2);
						r->ulamek = 0.5;
						res.push_back(r);
						graniczy = true;
						break;
					}
				}
				if (!graniczy)
				{
					float wrogie_ludki = 0;
					for (Domek* romeczek : domek1.drogi)
						if (romeczek->gracz->numer != gracz.numer&&romeczek->gracz->numer != 0)
						{
							wrogie_ludki += romeczek->liczebnosc*((float)(std::get<2>(rozgrywka.SilaGracza(romeczek->gracz->numer))) + 10.0) / 10.0;
						}
					if (domek1.poziom>=1&&2.0 * domek1.liczebnosc >= domek1.max_liczebnosc && (2.0 * domek1.liczebnosc - domek1.max_liczebnosc >= 2.0 * wrogie_ludki||(domek1.typdomku==TypDomku::kFort&&2.0 * domek1.liczebnosc *domek1.poziom - domek1.max_liczebnosc* domek1.poziom >= 2.0 * wrogie_ludki))&&domek1.poziom <= 4)
					{
						UlepszRozkaz* r = new UlepszRozkaz(&domek1);
						res.push_back(r);
						ruch = true;
					}
					else if (domek1.poziom == 0&&domek1.liczebnosc*2.0>=domek1.max_liczebnosc)
					{
						PrzebudujRozkaz* r = new PrzebudujRozkaz(&domek1, TypDomku::kFort);
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
					if (!ruch && !graniczy)
					{
						int min_poziom = domek1.poziom;
						Domek* ktojestromek = NULL;
						for (Domek* romek1 : domek1.drogi)
						{
							if (romek1->poziom < min_poziom)
							{
								min_poziom = romek1->poziom;
								ktojestromek = romek1;
							}
						}
						if (min_poziom < domek1.poziom )
						{
							auto r = new WymarszRozkaz(&domek1, ktojestromek);
							r->ulamek = 0.5;
							res.push_back(r);
						}
						else if (min_poziom == domek1.poziom&&domek1.liczebnosc*2.0>=domek1.max_liczebnosc&&domek1.typdomku==TypDomku::kFort)
						{
							PrzebudujRozkaz* r = new PrzebudujRozkaz(&domek1, TypDomku::kMiasto);
							res.push_back(r);
						}
						else if (min_poziom == domek1.poziom&&domek1.liczebnosc*2.0 >= domek1.max_liczebnosc&&domek1.typdomku == TypDomku::kMiasto&& (float)(std::get<2>(rozgrywka.SilaGracza(gracz.numer)))*3.0<= (float)(std::get<1>(rozgrywka.SilaGracza(gracz.numer)))&& (float)gracz.liczba_tworow * 2.0/5.0 >= (float)(std::get<1>(rozgrywka.SilaGracza(gracz.numer))))
							{
							PrzebudujRozkaz* r = new PrzebudujRozkaz(&domek1, TypDomku::kZbrojownia);
							res.push_back(r);
							}
						else if (min_poziom == domek1.poziom&&domek1.liczebnosc*2.0 >= domek1.max_liczebnosc&&domek1.typdomku == TypDomku::kZbrojownia && (float)(std::get<2>(rozgrywka.SilaGracza(gracz.numer))) * 2.0 > (float)(std::get<1>(rozgrywka.SilaGracza(gracz.numer))))
						{
							PrzebudujRozkaz* r = new PrzebudujRozkaz(&domek1, TypDomku::kMiasto);
							res.push_back(r);
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
	if (czas*szybkosc_komputera >= 1.0)
	{
		czas -= 1.0;
		bool ruch, graniczy;
		double odleglosc = 0;
		Domek* domekx = NULL;
		for (Domek& domek1 : rozgrywka.domki)
			if (domek1.gracz->numer == gracz.numer && 10 * domek1.liczebnosc >= domek1.max_liczebnosc)
				{
					graniczy = false;
					for (Domek* domek2 : domek1.drogi)
					{
						if (domek2->gracz->numer != gracz.numer && ile_trzeba_ludkow(rozgrywka.Odleglosc(domek1, (*domek2)), domek2) + 1.0 <= domek1.liczebnosc*(10.0 + (float)(std::get<2>(rozgrywka.SilaGracza(gracz.numer)))) / 10.0)
						{
							auto r = new WymarszRozkaz(&domek1, domek2);
							r->ulamek = 1.0;
							res.push_back(r);
							graniczy = true;
							break;
						}
					}
					if (!graniczy)
					{
						float wrogie_ludki = 0;
						for (Domek* romeczek : domek1.drogi)
							if (romeczek->gracz->numer != gracz.numer&&romeczek->gracz->numer != 0)
							{
								wrogie_ludki += romeczek->liczebnosc*((float)(std::get<2>(rozgrywka.SilaGracza(romeczek->gracz->numer))) + 10.0) / 10.0;
							}
						if (domek1.poziom >= 1 && 2.0 * domek1.liczebnosc >= domek1.max_liczebnosc && (2.0 * domek1.liczebnosc - domek1.max_liczebnosc >= 2.0 * wrogie_ludki || (domek1.typdomku == TypDomku::kFort&&2.0 * domek1.liczebnosc *domek1.poziom - domek1.max_liczebnosc* domek1.poziom >= 2.0 * wrogie_ludki)) && domek1.poziom <= 4)
						{
							UlepszRozkaz* r = new UlepszRozkaz(&domek1);
							res.push_back(r);
							ruch = true;
						}
						else if (domek1.poziom == 0 && domek1.liczebnosc*2.0 >= domek1.max_liczebnosc)
						{
							PrzebudujRozkaz* r = new PrzebudujRozkaz(&domek1, TypDomku::kFort);
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
						if (!ruch && !graniczy)
						{
							int min_poziom = domek1.poziom;
							Domek* ktojestromek = NULL;
							for (Domek* romek1 : domek1.drogi)
							{
								if (romek1->poziom < min_poziom)
								{
									min_poziom = romek1->poziom;
									ktojestromek = romek1;
								}
							}
							if (min_poziom < domek1.poziom)
							{
								auto r = new WymarszRozkaz(&domek1, ktojestromek);
								r->ulamek = 1.0;
								res.push_back(r);
							}
							else if (min_poziom == domek1.poziom&&domek1.liczebnosc*2.0 >= domek1.max_liczebnosc&&domek1.typdomku == TypDomku::kFort)
							{
								PrzebudujRozkaz* r = new PrzebudujRozkaz(&domek1, TypDomku::kMiasto);
								res.push_back(r);
							}
							else if (min_poziom == domek1.poziom&&domek1.liczebnosc*2.0 >= domek1.max_liczebnosc&&domek1.typdomku == TypDomku::kMiasto && (float)(std::get<2>(rozgrywka.SilaGracza(gracz.numer)))*3.0 <= (float)(std::get<1>(rozgrywka.SilaGracza(gracz.numer))) && (float)gracz.liczba_tworow * 2.0 / 5.0 >= (float)(std::get<1>(rozgrywka.SilaGracza(gracz.numer))))
							{
								PrzebudujRozkaz* r = new PrzebudujRozkaz(&domek1, TypDomku::kZbrojownia);
								res.push_back(r);
							}
							else if (min_poziom == domek1.poziom&&domek1.liczebnosc*2.0 >= domek1.max_liczebnosc&&domek1.typdomku == TypDomku::kZbrojownia && (float)(std::get<2>(rozgrywka.SilaGracza(gracz.numer))) * 2.0 > (float)(std::get<1>(rozgrywka.SilaGracza(gracz.numer))))
							{
								PrzebudujRozkaz* r = new PrzebudujRozkaz(&domek1, TypDomku::kMiasto);
								res.push_back(r);
							}
						}
					}
				}
	}
	return res;

}
