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
				else if (wybrany != nullptr && wybrany == klikniety)
				{
					klikniecia.clear();
					cel = (Domek*)klikniety;
				}
			}
		}
		else
		{
			klikniecia.clear();
			cel = nullptr;
			wybrany = nullptr;
			klikniecia.push_back(clock());
		}
	}

	if (zdarzenie.type == sf::Event::KeyPressed)
	{
		if (wybrany != nullptr && wybrany->gracz == &gracz)
		{
			if (zdarzenie.key.code == sf::Keyboard::B)
				nacisniety = 'B';
			else if (zdarzenie.key.code == sf::Keyboard::Z)
				nacisniety = 'Z';
			else if (zdarzenie.key.code == sf::Keyboard::K)
				nacisniety = 'K';
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
		nacisniety = 0;
	}
	else if (wybrany != nullptr&&cel != nullptr&&cel == wybrany&&wybrany->liczebnosc>=wybrany->max_liczebnosc/2)
	{
		auto r = new UlepszRozkaz(wybrany);
		res.push_back(r);

		wybrany = nullptr;
		cel = nullptr;
		nacisniety = 0;
	}
	// po 0.5 sekundy wysy�ane s� ludki
	if (cel != nullptr && cel != wybrany && clock() - klikniecia.back() > 0.2 * CLOCKS_PER_SEC)
	{
		double frakcja = 1;
		if (klikniecia.size() == 1)
			frakcja = 0.5;
		else if (klikniecia.size() == 2)
			frakcja = 1;

		// utworz i zwroc rozkaz 
		auto r = new WymarszRozkaz(wybrany, cel);
		r->ulamek = frakcja;
		res.push_back(r);

		wybrany = nullptr;
		cel = nullptr;
		nacisniety = 0;
		klikniecia.clear();
	}

	if (nacisniety != 0 && wybrany != nullptr)
	{
		switch (nacisniety)
		{
		case 'B':
			res.push_back(new PrzebudujRozkaz(wybrany, TypDomku::kOsada));
			break;
		case 'Z':
			res.push_back(new PrzebudujRozkaz(wybrany, TypDomku::kZamek));
			break;
		case 'K':
			res.push_back(new PrzebudujRozkaz(wybrany, TypDomku::kKuznia));
			break;
		}
		wybrany = nullptr;
		cel = nullptr;
		nacisniety = 0;
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
	Strzelaj(czas);
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

			auto liczba = int(ruch.skad->liczebnosc * ruch.ulamek);
			if (liczba > 0 && &ruch.skad != &ruch.dokad)
			{
				if (ruch.skad->drogi.size() == 0 || count(ruch.skad->drogi.begin(), ruch.skad->drogi.end(), ruch.dokad))
				{
					rozgrywka->ZmienLiczebnosc(*ruch.skad, ruch.skad->liczebnosc - liczba);

					rozgrywka->armie.push_back(Ludek(*ruch.dokad));
					Ludek& nowaArmia = rozgrywka->armie.back();
					nowaArmia.gracz = ruch.skad->gracz;
					nowaArmia.polozenie = ruch.skad->polozenie;
					nowaArmia.wyglad = Wyglad::kLudek;
					rozgrywka->ZmienLiczebnosc(nowaArmia, liczba);

					//ZMIERZ SILE LUDKOW I NADAJ IM SZYBKOSC
					int sila_ludkow=0;
					float wspolczynnik_walki=0.1,wspolczynnik_ruchu=0.1;
					for (Domek& domekek : rozgrywka->domki)
					{
						if (domekek.gracz->numer == ruch.skad->gracz->numer&&domekek.typdomku == TypDomku::kKuznia)sila_ludkow += domekek.poziom;
					}
					nowaArmia.tarcza = (int)((float)sila_ludkow*wspolczynnik_walki*(float)liczba);
					nowaArmia.szybkosc_ludka = sila_ludkow*wspolczynnik_ruchu + 1.0;

					ruch.skad->gracz->liczba_tworow++;
				}
			}
		}
		else if (IsType<UlepszRozkaz>(r))
		{
			auto ulepsz = (UlepszRozkaz*)r;

			// TODO sprawdz czy nie oszukuje ktoś (czy ma wystarczająco ludków)
			if (ulepsz->kogo->liczebnosc - ulepsz->kogo->max_liczebnosc / 2.0 > 0)
				{
				rozgrywka->ZmienLiczebnosc(*ulepsz->kogo, ulepsz->kogo->liczebnosc - ulepsz->kogo->max_liczebnosc / 2.0);
				ulepsz->kogo->poziom++;
				ulepsz->kogo->max_liczebnosc = 2 * ulepsz->kogo->max_liczebnosc;
				}
		}
		else if (IsType<PrzebudujRozkaz>(r))
		{
			auto przebuduj = (PrzebudujRozkaz*)r;
			if (przebuduj->kogo->liczebnosc >= 50.0)
			{
				przebuduj->kogo->typdomku = przebuduj->naco;
				przebuduj->kogo->poziom = 1;
				rozgrywka->ZmienLiczebnosc(*przebuduj->kogo, przebuduj->kogo->liczebnosc-50.0);
				przebuduj->kogo->max_liczebnosc = 100.0;
			}
			// TODO ustaw odpowiednio wartości jeśli ulepszenie jest możliwe
		}
	}
	kolejka_do_wykonania.clear();
}

void Ruszacz::PrzesuwajLudkow(float czas)
{
	double przesuniecie = szybkosc * szybkosc_ruchu;
	for (Ludek& armia : rozgrywka->armie)
	{
		if (rozgrywka->Spotkanie(armia) == NULL)
		{
			PD polozenie_cel = armia.cel->polozenie;
			PD polozenie_teraz = armia.polozenie;

			PD wektor_do_celu = (polozenie_cel - polozenie_teraz);
			double dlugosc = sqrt(wektor_do_celu.x * wektor_do_celu.x + wektor_do_celu.y * wektor_do_celu.y);
			PD jednostkowy = wektor_do_celu / dlugosc;
			jednostkowy *= przesuniecie*armia.szybkosc_ludka*czas;

			armia.polozenie += jednostkowy;
		}
	}
}

void Ruszacz::WalczLudkami(float czas)
{
	vector<Ludek*> do_usuniecia;
	for (auto it = rozgrywka->armie.begin(); it != rozgrywka->armie.end(); it++)
	{
		Ludek& armia = *it;
		double odleglosc = rozgrywka->Odleglosc(armia, *armia.cel);
		if (odleglosc < armia.cel->rozmiar)
		{
			if (IsType<Domek>(armia.cel))
			{
				Domek* cel = (Domek*)armia.cel;
				if (armia.gracz == armia.cel->gracz)
				{
					rozgrywka->ZmienLiczebnosc(*cel, armia.liczebnosc + cel->liczebnosc);
				}
				else
				{
					double nowa_liczebnosc;
					if (cel->typdomku == TypDomku::kZamek)
						{
						cel->liczebnosc = std::max(0.0, cel->liczebnosc - (double)armia.tarcza/(double)cel->poziom);
						nowa_liczebnosc = cel->liczebnosc - armia.liczebnosc/(double)cel->poziom;
						}
					else
						{
						cel->liczebnosc = std::max(0.0, cel->liczebnosc - (double)armia.tarcza);
						nowa_liczebnosc = cel->liczebnosc - armia.liczebnosc;
						}
					if (nowa_liczebnosc < 0)
					{
						rozgrywka->ZabierzTwor(cel);
						cel->gracz = armia.gracz;
						armia.gracz->liczba_tworow++;						
					}
					rozgrywka->ZmienLiczebnosc(*cel, std::abs(nowa_liczebnosc));
				}

				rozgrywka->ZmienLiczebnosc(armia, std::abs(0));
				do_usuniecia.push_back(&(*it));
			}
			// jak to nie jest domek to nic nie r�bmy, mo�e kiedy� b�dziemy?
		}
		else {
			auto spotkanie = rozgrywka->Spotkanie(armia);
			if (spotkanie != NULL)
			{
				rozgrywka->TracLudki(armia, std::max(5.0, 5 * czas * szybkosc));
				rozgrywka->TracLudki(*spotkanie, std::max(5.0, 5 * czas * szybkosc));
		
				if (armia.liczebnosc <= 0)
				{
					do_usuniecia.push_back(&(*it));
				}

				if (spotkanie->liczebnosc <= 0)
				{
					do_usuniecia.push_back(spotkanie);
				}
			}
		}
	}

	for (auto usunieta : do_usuniecia)
	{
		rozgrywka->ZniszczLudka(usunieta);
	}
}

void Ruszacz::Produkuj(float czas)
{
	for (Domek& domek : rozgrywka->domki)
	{
		if (domek.gracz->aktywny&&domek.liczebnosc == domek.max_liczebnosc);
		else if(domek.gracz->aktywny&&domek.liczebnosc<domek.max_liczebnosc&&domek.typdomku == TypDomku::kOsada)rozgrywka->ZmienLiczebnosc(domek, domek.liczebnosc + szybkosc*czas*domek.produkcja*domek.poziom);
		else if(domek.gracz->aktywny&&domek.liczebnosc>domek.max_liczebnosc)rozgrywka->ZmienLiczebnosc(domek,max(domek.liczebnosc - szybkosc*czas*5,(double)domek.max_liczebnosc));
	}
}

void Ruszacz::Strzelaj(float czas)
{
	int sila_strzalu = 5.0;
	//tego na razie nie ma
	vector<Ludek*> do_usuniecia;
	for (Ludek& ludek : rozgrywka->armie)
		{
		Domek * domek_cel = ((Domek*)ludek.cel);
		if (domek_cel->typdomku==TypDomku::kZamek&&domek_cel->gracz->numer!=ludek.gracz->numer&&rozgrywka->Odleglosc(ludek,*domek_cel) < 100.0)
			{
			ludek.liczebnosc = std::max(0.0, ludek.liczebnosc - sila_strzalu*(double)domek_cel->poziom*czas);
			if(ludek.liczebnosc==0.0)do_usuniecia.push_back(&ludek);
			}
		}
	for (auto usunieta : do_usuniecia)
	{
		rozgrywka->ZniszczLudka(usunieta);
	}
}

WymarszRozkaz::WymarszRozkaz(Domek * skad, Domek * dokad) : skad(skad), dokad(dokad)
{
}

UlepszRozkaz::UlepszRozkaz(Domek * kogo) : kogo(kogo)
{
}

PrzebudujRozkaz::PrzebudujRozkaz(Domek * kogo, TypDomku naco) : kogo(kogo), naco(naco)
{
}
