#include "dzialacze.h"
#include <set>
#include <string>
#include<ctime>
#include<cmath>

MyszDecydent::MyszDecydent(sf::RenderWindow & okno, Rozgrywka & rozgrywka, Gracz & gracz) : okno(okno), rozgrywka(rozgrywka), gracz(gracz)
{
}

void MyszDecydent::Klik(double x, double y)
{
}

void MyszDecydent::Przetworz(sf::Event zdarzenie)
{
	if (zdarzenie.type == sf::Event::MouseButtonPressed)
	{
		if (zdarzenie.mouseButton.button == sf::Mouse::Left)
		{
			sf::Vector2i pixelPos = sf::Mouse::getPosition(okno);
			auto polozenie_kliku = okno.mapPixelToCoords(pixelPos);
			Twor* klikniety = rozgrywka.Zlokalizuj(polozenie_kliku.x, polozenie_kliku.y);
			if (klikniety != nullptr && IsType<Domek>(klikniety))
			{
				if (wybrany == nullptr && klikniety->gracz->numer == gracz.numer)
					wybrany = (Domek*)klikniety;
				else if (wybrany != nullptr && wybrany != klikniety)
				{
					if (cel != klikniety)
					{
						klikniecia.clear();
						cel = (Domek*)klikniety;
					}
					klikniecia.push_back(clock());
				}
			}
		}
		else
		{
			klikniecia.clear();
			cel = nullptr;
			wybrany = nullptr;
		}
	}
}

vector<Rozkaz*> MyszDecydent::WykonajRuch()
{
	vector<Rozkaz*> res;
	if (wybrany != nullptr && wybrany->gracz != &gracz)
	{
		cel = nullptr;
		wybrany = nullptr;
	}

	// po 0.5 sekundy wysy�ane s� ludki
	if (cel != nullptr && clock() - klikniecia.back() > 0.2 * CLOCKS_PER_SEC)
	{
		double frakcja = 1;
		if (klikniecia.size() == 1)
			frakcja = 0.5;
		else if (klikniecia.size() == 2)
			frakcja = 1;

		// utworz i zwroc rozkaz 
		auto r = new WymarszRozkaz(*wybrany, *cel);
		r->ulamek = frakcja;
		res.push_back(r);

		wybrany = nullptr;
		cel = nullptr;
		klikniecia.clear();
	}
	return res;
}


Ruszacz::Ruszacz()
{
}

void Ruszacz::Ruszaj(float czas)
{
	WykonajRuchy();
	PrzesuwajLudkow(czas);
	WalczLudkami(czas);
	Produkuj(czas);
}

void Ruszacz::PrzyjmijRuch(vector<Rozkaz*> rozkazy)
{
	kolejka_do_wykonania.insert(kolejka_do_wykonania.end(), rozkazy.begin(), rozkazy.end());
}

void Ruszacz::WykonajRuchy()
{
	for (auto r : kolejka_do_wykonania)
	{
		if (IsType<WymarszRozkaz>(r))
		{
			auto& ruch = *(WymarszRozkaz*)r;

			auto liczba = int(ruch.skad.liczebnosc * ruch.ulamek);
			if (liczba > 0 && &ruch.skad != &ruch.dokad)
			{
				rozgrywka->ZmienLiczebnosc(ruch.skad, ruch.skad.liczebnosc - liczba);

				rozgrywka->armie.push_back(Ludek(ruch.dokad));
				Ludek& nowaArmia = rozgrywka->armie.back();
				nowaArmia.gracz = ruch.skad.gracz;
				nowaArmia.polozenie = ruch.skad.polozenie;
				nowaArmia.wyglad = Wyglad::kLudek;
				rozgrywka->ZmienLiczebnosc(nowaArmia, liczba);

				ruch.skad.gracz->liczba_tworow++;
			}
		}
	}
	kolejka_do_wykonania.clear();
}

void Ruszacz::PrzesuwajLudkow(float czas)
{
	double przesuniecie = szybkosc * szybkosc_ruchu;
	for (Ludek& armia : rozgrywka->armie)
	{
		
		PD polozenie_cel = armia.cel.polozenie;
		PD polozenie_teraz = armia.polozenie;

		PD wektor_do_celu = (polozenie_cel - polozenie_teraz);
		double dlugosc = sqrt(wektor_do_celu.x * wektor_do_celu.x + wektor_do_celu.y * wektor_do_celu.y);
		PD jednostkowy = wektor_do_celu / dlugosc;
		jednostkowy *= przesuniecie*czas;

		armia.polozenie += jednostkowy;
	}
}

void Ruszacz::WalczLudkami(float czas)
{
	vector<list<Ludek>::iterator> do_usuniecia;
	for (auto it = rozgrywka->armie.begin(); it != rozgrywka->armie.end(); it++)
	{
		Ludek& armia = *it;
		double odleglosc = rozgrywka->Odleglosc(armia, armia.cel);
		if (odleglosc < armia.cel.rozmiar)
		{
			if (IsType<Domek>(&armia.cel))
			{
				Domek& cel = (Domek&)armia.cel;
				if (armia.gracz == armia.cel.gracz)
				{
					rozgrywka->ZmienLiczebnosc(cel, armia.liczebnosc + cel.liczebnosc);
				}
				else
				{
					double nowa_liczebnosc = cel.liczebnosc - armia.liczebnosc;
					if (nowa_liczebnosc < 0)
					{
						cel.gracz->liczba_tworow--;
						if (cel.gracz->liczba_tworow == 0)
						{
							if (cel.gracz->aktywny)rozgrywka->liczba_aktywnych_graczy--;
							cel.gracz->aktywny = false;
						}
						cel.gracz = armia.gracz;
						armia.gracz->liczba_tworow++;						
					}
					rozgrywka->ZmienLiczebnosc(cel, std::abs(nowa_liczebnosc));
				}
				armia.gracz->liczba_tworow--;
				if (armia.gracz->liczba_tworow == 0)
				{
					if(armia.gracz->aktywny)rozgrywka->liczba_aktywnych_graczy--;
					armia.gracz->aktywny = false;
				}
				rozgrywka->ZmienLiczebnosc(armia, std::abs(0));
				do_usuniecia.push_back(it);
			}
			// jak to nie jest domek to nic nie r�bmy, mo�e kiedy� b�dziemy?
		}
	}

	for (auto usunieta : do_usuniecia)
	{
		rozgrywka->armie.erase(usunieta);
	}
}

void Ruszacz::Produkuj(float czas)
{
	for (Domek& domek : rozgrywka->domki)
	{
		if(domek.gracz->aktywny)rozgrywka->ZmienLiczebnosc(domek, domek.liczebnosc + szybkosc*czas*domek.produkcja);
	}
}

WymarszRozkaz::WymarszRozkaz(Domek & skad, Domek & dokad) : skad(skad), dokad(dokad)
{
}
