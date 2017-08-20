#include "dzialacze.h"
#include <set>

MyszDecydent::MyszDecydent(Rozgrywka & rozgrywka, Gracz & gracz) : rozgrywka(rozgrywka), gracz(gracz)
{
}

void MyszDecydent::Klik(double x, double y)
{
}

void MyszDecydent::Przetworz(sf::Event zdarzenie)
{
}

void MyszDecydent::WykonajRuch()
{
}



Ruszacz::Ruszacz(Rozgrywka & rozgrywka) : rozgrywka(rozgrywka)
{
}

void Ruszacz::Ruszaj()
{
	PrzesuwajLudkow();
	WalczLudkami();
	Produkuj();
}

void Ruszacz::PrzesuwajLudkow()
{
	double przesuniecie = szybkosc * szybkosc_ruchu;
	for (Ludek& armia : rozgrywka.armie)
	{
		PD polozenie_cel = armia.cel.polozenie;
		PD polozenie_teraz = armia.polozenie;

		PD wektor_do_celu = (polozenie_cel - polozenie_teraz);
		double dlugosc = sqrt(wektor_do_celu.x * wektor_do_celu.x + wektor_do_celu.y * wektor_do_celu.y);
		PD jednostkowy = wektor_do_celu / dlugosc;
		jednostkowy *= przesuniecie;

		armia.polozenie += jednostkowy;
	}
}

void Ruszacz::WalczLudkami()
{
}

void Ruszacz::Produkuj()
{
}

Wyswietlacz::Wyswietlacz(Rozgrywka & rozgrywka) : rozgrywka(rozgrywka)
{
	
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
		wyglad.setFillColor(twor->gracz->numer == 1 ? sf::Color::Green : sf::Color::Red);
		okno.draw(wyglad);
	}
}
