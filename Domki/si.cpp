#include "dzialacze.h"
#include"si.h"
#include <set>
#include <string>
#include<ctime>
#include<cmath>

Komputer::Komputer(Rozgrywka & rozgrywka, Gracz & gracz, double szybkosc_komputera) : rozgrywka(rozgrywka), gracz(gracz), szybkosc_komputera(szybkosc_komputera)
{
	czas = (double)(rand() % 30) / 10.0;
}

double ile_trzeba_ludkow(double odleglosc, Domek* domek,Rozgrywka& rozgrywka)
{
	double przyrostarmii = 2.0;
	double przesuniecie = 200.0 / 3.0;
	if (domek->typdomku == TypDomku::kMiasto&&domek->gracz->aktywny == true)return domek->liczebnosc + przyrostarmii * domek->poziom * odleglosc / przesuniecie + 1.0;
	else if (domek->typdomku == TypDomku::kFort)return domek->liczebnosc*(domek->poziom + 1.0) + 1.0;
    else if (domek->typdomku == TypDomku::kWieza)domek->liczebnosc*6.0/(6.0-domek->poziom) + 1.0;
	return domek->liczebnosc + 1.0;
}

double otoczenie(Domek* domek, Rozgrywka& rozgrywka,bool neutralny) {
    double wojownicy_otaczajacy = 0.0;
    for (Domek* sasiad : domek->drogi) {
        if (sasiad->gracz != domek->gracz&&(sasiad->gracz->numer !=0||neutralny))wojownicy_otaczajacy += sasiad->liczebnosc;
    }
    return wojownicy_otaczajacy;
}

double bliscy_wrogowie(Domek* domek, Rozgrywka& rozgrywka,bool neutralny) {
    double wojownicy_bliscy = 0.0;
    for (Domek* sasiad : domek->drogi) {
        wojownicy_bliscy+= otoczenie(sasiad,rozgrywka,neutralny);
    }
    return wojownicy_bliscy;
}

vector<Rozkaz*> Komputer::WykonajRuch()
{
    vector<Rozkaz*> res;
    bool ruch;
    if (czas >= (11.0 - szybkosc_komputera)*(1.0+(double)gracz.liczba_tworow/10.0))
    {
        czas -= (11.0 - szybkosc_komputera)*(1.0 + (double)gracz.liczba_tworow / 10.0);
        for (Domek& domek : rozgrywka.domki) if (domek.gracz->numer == gracz.numer) {
            ruch = true;
            if (otoczenie(&domek, rozgrywka, false) > 0.0) {//sasiad to wrog
                double ludki = 0.0;
                Domek* ktojestromek = NULL;
                for (Domek* romek : domek.drogi) if (romek->gracz->numer != gracz.numer&&romek->gracz->numer > 0) {
                    if (2 * ile_trzeba_ludkow(rozgrywka.Odleglosc(domek, (*romek)), romek, rozgrywka) <= domek.liczebnosc&&ile_trzeba_ludkow(rozgrywka.Odleglosc(domek, (*romek)), romek, rozgrywka) > ludki) {
                        ludki = ile_trzeba_ludkow(rozgrywka.Odleglosc(domek, (*romek)), romek, rozgrywka);
                        ktojestromek = romek;
                    }
                }
                if (ludki > 0.0) {
                    auto r = new WymarszRozkaz(&domek, ktojestromek);
                    r->ulamek = 0.5;
                    res.push_back(r);
                    ruch = false;
                }
                if (ruch) {
                    for (Domek* romek : domek.drogi) if (romek->gracz->numer != gracz.numer&&romek->gracz->numer > 0) {
                        if (ile_trzeba_ludkow(rozgrywka.Odleglosc(domek, (*romek)), romek, rozgrywka) <= domek.liczebnosc&&ile_trzeba_ludkow(rozgrywka.Odleglosc(domek, (*romek)), romek, rozgrywka) > ludki) {
                            ludki = ile_trzeba_ludkow(rozgrywka.Odleglosc(domek, (*romek)), romek, rozgrywka);
                            ktojestromek = romek;
                        }
                    }
                    if (ludki > 0.0) {
                        auto r = new WymarszRozkaz(&domek, ktojestromek);
                        r->ulamek = 1.0;
                        res.push_back(r);
                        ruch = false;
                    }
                }
                if (ruch&&domek.liczebnosc * 4 >= domek.max_liczebnosc && (domek.typdomku == TypDomku::kMiasto || domek.typdomku == TypDomku::kWieza)) {
                    for (Domek* romek : domek.drogi) if (romek->gracz->numer != gracz.numer&&romek->gracz->numer > 0) {
                        auto r = new WymarszRozkaz(&domek, ktojestromek);
                        r->ulamek = 1.0;
                        res.push_back(r);
                        ruch = false;
                        break;
                    }
                }
                else if (ruch&&domek.liczebnosc * 2 >= domek.max_liczebnosc && (domek.typdomku == TypDomku::kStajnia || domek.typdomku == TypDomku::kZbrojownia)) {
                    PrzebudujRozkaz* r = new PrzebudujRozkaz(&domek, TypDomku::kMiasto);
                    for (Domek* romek : domek.drogi)if (romek->gracz->numer != gracz.numer&&romek->gracz->numer > 0 && otoczenie(&domek, rozgrywka, false) > otoczenie(romek, rozgrywka, false)) {
                        r = new PrzebudujRozkaz(&domek, TypDomku::kWieza);
                        break;
                    }
                    res.push_back(r);
                    ruch = false;
                }
                else if (ruch&&domek.liczebnosc * 2.0 >= domek.max_liczebnosc&&domek.poziom < 5) {
                    if (domek.typdomku != TypDomku::kMiasto) {
                        PrzebudujRozkaz* r = new PrzebudujRozkaz(&domek, TypDomku::kMiasto);
                        res.push_back(r);
                    }
                    else {
                        UlepszRozkaz* r = new UlepszRozkaz(&domek);
                        res.push_back(r);
                    }
                    ruch = false;
                }
            }
            else if (otoczenie(&domek, rozgrywka, true) > 0.0) {//neutralny sasiad
                double ludki = 0.0;
                int poziom = 0;
                Domek* ktojestromek = NULL;
                for (Domek* romek : domek.drogi) {
                    if (romek->gracz->numer == 0 && 2 * ile_trzeba_ludkow(rozgrywka.Odleglosc(domek, (*romek)), romek, rozgrywka) <= domek.liczebnosc&&ile_trzeba_ludkow(rozgrywka.Odleglosc(domek, (*romek)), romek, rozgrywka) > ludki) {
                        ludki = ile_trzeba_ludkow(rozgrywka.Odleglosc(domek, (*romek)), romek, rozgrywka);
                        ktojestromek = romek;
                    }

                }
                if (ludki > 0.0) {
                    auto r = new WymarszRozkaz(&domek, ktojestromek);
                    r->ulamek = 0.5;
                    res.push_back(r);
                    ruch = false;
                }
                if (ruch) {
                    for (Domek* romek : domek.drogi) {
                        if (romek->gracz->numer == 0 && ile_trzeba_ludkow(rozgrywka.Odleglosc(domek, (*romek)), romek, rozgrywka) <= domek.liczebnosc&&ile_trzeba_ludkow(rozgrywka.Odleglosc(domek, (*romek)), romek, rozgrywka) > ludki) {
                            ludki = ile_trzeba_ludkow(rozgrywka.Odleglosc(domek, (*romek)), romek, rozgrywka);
                            ktojestromek = romek;
                        }
                    }
                    if (ludki > 0.0) {
                        auto r = new WymarszRozkaz(&domek, ktojestromek);
                        r->ulamek = 1.0;
                        res.push_back(r);
                        ruch = false;
                    }
                }
                if (ruch&&domek.liczebnosc * 2.0 >= domek.max_liczebnosc&&domek.poziom < 5) {
                    if (domek.typdomku != TypDomku::kMiasto) {
                        PrzebudujRozkaz* r = new PrzebudujRozkaz(&domek, TypDomku::kMiasto);
                        res.push_back(r);
                    }
                    else {
                        UlepszRozkaz* r = new UlepszRozkaz(&domek);
                        res.push_back(r);
                    }
                    ruch = false;
                }
                else if (ruch&&domek.liczebnosc * 2.0 > domek.max_liczebnosc && (domek.typdomku == TypDomku::kMiasto || domek.typdomku == TypDomku::kWieza)) {
                    for (Domek* romek : domek.drogi) if (romek->gracz->numer != gracz.numer) {
                        auto r = new WymarszRozkaz(&domek, ktojestromek);
                        r->ulamek = 1.0;
                        res.push_back(r);
                        ruch = false;
                        break;
                    }
                }
            }
            else if (bliscy_wrogowie(&domek, rozgrywka, false) > 0.0) {//bliski wrog
                if (!(domek.typdomku == TypDomku::kFort || domek.typdomku == TypDomku::kWieza) && domek.liczebnosc*2.0 >= domek.max_liczebnosc) {
                    UlepszRozkaz* r = new UlepszRozkaz(&domek);
                    res.push_back(r);
                }
                int max_otoczenie = 0.0;
                Domek* ktojestromek = NULL;
                for (Domek* romek : domek.drogi)
                {
                    if (otoczenie(romek, rozgrywka, false) > max_otoczenie)
                    {
                        max_otoczenie = otoczenie(romek, rozgrywka, false);
                        ktojestromek = romek;
                    }
                }
                auto r = new WymarszRozkaz(&domek, ktojestromek);
                r->ulamek = 1.0;
                res.push_back(r);
                ruch = false;
            }
            else if (bliscy_wrogowie(&domek, rozgrywka, true) > 0.0) {//bliski neutralny
                if (!(domek.typdomku == TypDomku::kFort || domek.typdomku == TypDomku::kWieza) && domek.liczebnosc*2.0 >= domek.max_liczebnosc) {
                    UlepszRozkaz* r = new UlepszRozkaz(&domek);
                    res.push_back(r);
                }
                else if (domek.liczebnosc * 4 >= domek.max_liczebnosc) {
                    int max_otoczenie = 0.0;
                    Domek* ktojestromek = NULL;
                    for (Domek* romek : domek.drogi)
                    {
                        if (otoczenie(romek, rozgrywka, true) > max_otoczenie)
                        {
                            max_otoczenie = otoczenie(romek, rozgrywka, true);
                            ktojestromek = romek;
                        }
                    }
                    auto r = new WymarszRozkaz(&domek, ktojestromek);
                    r->ulamek = 1.0;
                    res.push_back(r);
                    ruch = false;
                }
            }
            else if (!(domek.typdomku == TypDomku::kFort || domek.typdomku == TypDomku::kWieza) && domek.liczebnosc*2.0 >= domek.max_liczebnosc) {//ulepszaj
                UlepszRozkaz* r = new UlepszRozkaz(&domek);
                res.push_back(r);
                ruch = false;
            }
            if (ruch) {//przebudowywuj lub stabilizuj
                int min_poziom = domek.poziom;
                Domek* ktojestromek = NULL;
                for (Domek* romek : domek.drogi)
                {
                    if (romek->poziom < min_poziom)
                    {
                        min_poziom = romek->poziom;
                        ktojestromek = romek;
                    }
                }
                if (min_poziom < domek.poziom)
                {
                    auto r = new WymarszRozkaz(&domek, ktojestromek);
                    r->ulamek = 0.5;
                    res.push_back(r);
                    ruch = false;
                }
                else if (min_poziom == domek.poziom&&domek.liczebnosc*2.0 >= domek.max_liczebnosc && (domek.typdomku == TypDomku::kFort || domek.typdomku == TypDomku::kWieza))
                {
                    PrzebudujRozkaz* r = new PrzebudujRozkaz(&domek, TypDomku::kMiasto);
                    res.push_back(r);
                    ruch = false;
                }
                else if (domek.typdomku == TypDomku::kFort || domek.typdomku == TypDomku::kWieza) {
                    BurzRozkaz* r = new BurzRozkaz(&domek);
                    res.push_back(r);
                    ruch = false;
                }
                else if (min_poziom == domek.poziom&&domek.liczebnosc*2.0 >= domek.max_liczebnosc&&domek.typdomku == TypDomku::kMiasto &&
                    (float)(std::get<2>(rozgrywka.SilaGracza(gracz.numer)))*3.0 <= (float)(std::get<1>(rozgrywka.SilaGracza(gracz.numer))) && (float)gracz.liczba_tworow * 2.0 / 5.0 >= (float)(std::get<1>(rozgrywka.SilaGracza(gracz.numer))))
                {
                    PrzebudujRozkaz* r = new PrzebudujRozkaz(&domek, TypDomku::kZbrojownia);
                    res.push_back(r);
                    ruch = false;
                }
                else if (min_poziom == domek.poziom&&domek.liczebnosc*2.0 >= domek.max_liczebnosc&&domek.typdomku == TypDomku::kZbrojownia &&
                    (float)(std::get<2>(rozgrywka.SilaGracza(gracz.numer))) * 2.0 > (float)(std::get<1>(rozgrywka.SilaGracza(gracz.numer))))
                {
                    PrzebudujRozkaz* r = new PrzebudujRozkaz(&domek, TypDomku::kMiasto);
                    res.push_back(r);
                    ruch = false;
                }
            }
        }
    }
    return res;
}

KomputerSilver::KomputerSilver(Rozgrywka & rozgrywka, Gracz & gracz, double szybkosc_komputera) : Komputer(rozgrywka, gracz, szybkosc_komputera)
{

}

//ile_trzeba_ludkow(rozgrywka.Odleglosc(domekZ, (*domekDO)), domekDO, rozgrywka)

vector<Rozkaz*> KomputerSilver::WykonajRuch()
{
    vector<Rozkaz*> res;
    return res;
}
