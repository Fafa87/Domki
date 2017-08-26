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
		auto liczba = int(wybrany->liczebnosc / 4);
		if (liczba > 0)
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
	obrazek_tworow[Wyglad::kDomek] = new sf::Texture();
	obrazek_tworow[Wyglad::kDomek]->loadFromFile("Grafika\\domek.png");
	obrazek_tworow[Wyglad::kDomek]->setSmooth(true);

	obrazek_tworow[Wyglad::kLudek] = new sf::Texture();
	obrazek_tworow[Wyglad::kLudek]->loadFromFile("Grafika\\ludek.png");
	obrazek_tworow[Wyglad::kLudek]->setSmooth(true);
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
		if (twor->wyglad == Wyglad::kDomek)
			wyglad.setFillColor(twor->gracz->numer == 1 ? sf::Color::Color(0, 255, 0, 255) : sf::Color::Color(255, 0, 0, 255));
		else if (twor->wyglad == Wyglad::kLudek)
			wyglad.setFillColor(twor->gracz->numer == 1 ? sf::Color::Color(10, 150, 50, 255) : sf::Color::Color(150, 10, 50, 255));

		if (twor->wyglad == Wyglad::kDomek || twor->wyglad == Wyglad::kLudek)
		{
			wyglad.setTexture(obrazek_tworow[twor->wyglad]);
		}
		okno.draw(wyglad);
	}
}
