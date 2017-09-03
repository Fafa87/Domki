#include "dzialacze.h"
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
		Domek* domekx=NULL;
		for (Domek& domek1 : rozgrywka.domki)
			if (!ruch&&domek1.gracz->numer == 2)
			{
				ruch = false;
				for (Domek& domek2 : rozgrywka.domki)
				{
					if (domek2.gracz->numer != 2 && ((domek2.gracz->numer == 3 && domek2.liczebnosc * 2 <= domek1.liczebnosc) || (domek2.gracz->numer == 1 && (domek1.liczebnosc >= (domek2.liczebnosc + licz_przyrost(rozgrywka.Odleglosc(domek1, domek2))) * 2 + 1)) && (odleglosc == 0 || rozgrywka.Odleglosc(domek1, domek2) < odleglosc)))
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
					}
				}
			}
	}
}

MyszDecydent::MyszDecydent(Rozgrywka & rozgrywka, Gracz & gracz) : rozgrywka(rozgrywka), gracz(gracz)
{
}

void MyszDecydent::Klik(double x, double y)
{
}

void MyszDecydent::Przetworz(sf::Event zdarzenie)
{
	if (zdarzenie.type == sf::Event::MouseButtonPressed && zdarzenie.mouseButton.button == sf::Mouse::Left)
	{
		Twor* klikniety = rozgrywka.Zlokalizuj(zdarzenie.mouseButton.x, zdarzenie.mouseButton.y);
		if (klikniety != nullptr && IsType<Domek>(klikniety))
		{
			if (wybrany == nullptr && klikniety->gracz->numer == gracz.numer)
				wybrany = (Domek*)klikniety;
			else if (wybrany != nullptr)
				cel = (Domek*)klikniety;
		}
	}
}

void MyszDecydent::WykonajRuch()
{
	if (cel != nullptr)
	{
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
		}

		wybrany = nullptr;
		cel = nullptr;
	}
}



Ruszacz::Ruszacz(Rozgrywka & rozgrywka) : rozgrywka(rozgrywka)
{
}

void Ruszacz::Ruszaj(float czas)
{
	PrzesuwajLudkow(czas);
	WalczLudkami(czas);
	Produkuj(czas);
}

void Ruszacz::PrzesuwajLudkow(float czas)
{
	double przesuniecie = szybkosc * szybkosc_ruchu;
	for (Ludek& armia : rozgrywka.armie)
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
	for (Ludek& armia : rozgrywka.armie)
	{
		double odleglosc = rozgrywka.Odleglosc(armia, armia.cel);
		if (odleglosc < armia.cel.rozmiar)
		{
			if (IsType<Domek>(&armia.cel))
			{
				Domek& cel = (Domek&)armia.cel;
				if (armia.gracz == armia.cel.gracz)
				{
					rozgrywka.ZmienLiczebnosc(cel, armia.liczebnosc + cel.liczebnosc);
				}
				else
				{
					double nowa_liczebnosc = cel.liczebnosc - armia.liczebnosc;
					if (nowa_liczebnosc < 0)
					{
						cel.gracz = armia.gracz;
					}
					rozgrywka.ZmienLiczebnosc(cel, std::abs(nowa_liczebnosc));
				}
				rozgrywka.ZmienLiczebnosc(armia, std::abs(0));
				/*list<Ludek>::iterator do_usuniecia;
				for (auto it = rozgrywka.armie.begin(); it != rozgrywka.armie.end(); it++)
				{
					if (&(*it) == &armia)
					{
						do_usuniecia = it;
						break;
					}
				}
				rozgrywka.armie.erase(do_usuniecia);
				break;*/
			}
			// jak to nie jest domek to nic nie róbmy, mo¿e kiedyœ bêdziemy?
		}
	}
}

void Ruszacz::Produkuj(float czas)
{
	for (Domek& domek : rozgrywka.domki)
	{
		if(domek.gracz->aktywny)rozgrywka.ZmienLiczebnosc(domek, domek.liczebnosc + szybkosc*czas*domek.produkcja);
	}
}

Wyswietlacz::Wyswietlacz(Rozgrywka & rozgrywka) : rozgrywka(rozgrywka)
{
	obrazek_tworow[Wyglad::kDomek] = new sf::Texture();
	obrazek_tworow[Wyglad::kDomek]->loadFromFile("Grafika\\domek_fala.gif");
	obrazek_tworow[Wyglad::kDomek]->setSmooth(true);

	obrazek_tworow[Wyglad::kLudek] = new sf::Texture();
	obrazek_tworow[Wyglad::kLudek]->loadFromFile("Grafika\\ludek.png");
	obrazek_tworow[Wyglad::kLudek]->setSmooth(true);

	czcionka.loadFromFile("Grafika\\waltographUI.ttf");
}

void Wyswietlacz::Wyswietlaj(sf::RenderWindow & okno)
{
	set<Twor*> wszystkie_obiekty;
	for (auto& dom : rozgrywka.domki)
		wszystkie_obiekty.insert(&dom);
	for (auto& lud : rozgrywka.armie)
		wszystkie_obiekty.insert(&lud);

	// usuñ wygl¹dy których ju¿ nie ma
	for (auto& wyg_map : wyglad_tworow)
		if (!wszystkie_obiekty.count(wyg_map.first))
			wyglad_tworow.erase(wyg_map.first);

	// dodaj wygl¹dy których brakuje
	for (auto& twor : wszystkie_obiekty)
	{
		if (!wyglad_tworow.count(twor))
		{
			// brakuje wiêc utworz nowy obiekt do wyœwietlania
			sf::CircleShape kolo(twor->rozmiar);
		}
	}

	// wygl¹d tworów zawiera dok³adnie to co chcemy wyœwietliæ, uaktualnijmy ich stan
	for (auto& twor : wszystkie_obiekty)
	{
		auto wyglad = wyglad_tworow[twor];
		wyglad.setPosition(twor->polozenie.x, twor->polozenie.y);
		wyglad.setRadius(twor->rozmiar);
		wyglad.setOrigin(twor->rozmiar, twor->rozmiar);
		wyglad.setFillColor(twor->gracz->kolor);
		if (twor->wyglad == Wyglad::kDomek || twor->wyglad == Wyglad::kLudek)
		{
			wyglad.setTexture(obrazek_tworow[twor->wyglad]);
		}

		sf::Text podpis;
		int liczba = 0;
		if (IsType<Domek>(twor))
			liczba = ((Domek*)twor)->liczebnosc;
		else if (IsType<Ludek>(twor))
			liczba = ((Ludek*)twor)->liczebnosc;
		podpis.setFont(czcionka);
		podpis.setCharacterSize(13);
		podpis.setString(std::to_string(liczba));
		podpis.setStyle(sf::Text::Bold);
		podpis.setFillColor(twor->gracz->kolor);
		podpis.move(twor->polozenie.x, twor->polozenie.y + twor->rozmiar);



		okno.draw(podpis);
		okno.draw(wyglad);
	}
}
