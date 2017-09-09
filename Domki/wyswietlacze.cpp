#include "Narzedzia\AnimatedSprite.hpp"
#include "wyswietlacze.h"

#include<set>
#include<string>
#include<ctime>
#include<cmath>

Wyswietlacz::Wyswietlacz(Rozgrywka & rozgrywka) : rozgrywka(rozgrywka)
{
	czcionka.loadFromFile("Grafika\\waltographUI.ttf");
}

void Wyswietlacz::Zaladuj(string wybrana_skora)
{
	skorka = wybrana_skora;
	obrazek_tworow[Wyglad::kDomek] = new sf::Texture();
	obrazek_tworow[Wyglad::kDomek]->loadFromFile("Grafika\\" + skorka + "\\kamienica.png");
	obrazek_tworow[Wyglad::kDomek]->setSmooth(true);

	obrazek_tworow[Wyglad::kLudek] = new sf::Texture();
	obrazek_tworow[Wyglad::kLudek]->loadFromFile("Grafika\\" + skorka + "\\krasnal.png");
	obrazek_tworow[Wyglad::kLudek]->setSmooth(true);

	obrazek_tla.loadFromFile("Grafika\\" + skorka + "\\bruk.png");
	obrazek_tla.setRepeated(true);
}

void Wyswietlacz::WyswietlTlo(sf::RenderWindow& okno)
{
	sf::Sprite wyglad_tlo(obrazek_tla);
	wyglad_tlo.setTextureRect({ 0, 0, 1600, 900 });
	okno.draw(wyglad_tlo);
}

void Wyswietlacz::Wyswietlaj(sf::RenderWindow & okno)
{
	set<Twor*> wszystkie_obiekty;
	for (auto& dom : rozgrywka.domki)
		wszystkie_obiekty.insert(&dom);
	for (auto& lud : rozgrywka.armie)
		wszystkie_obiekty.insert(&lud);

	// usuñ wygl¹dy których ju¿ nie ma
	vector<Twor*> do_usuniecia;
	for (auto& wyg_map : wyglad_tworow)
		if (!wszystkie_obiekty.count(wyg_map.first))
			do_usuniecia.push_back(wyg_map.first);

	for (auto twor : do_usuniecia)
		wyglad_tworow.erase(twor);

	// dodaj wygl¹dy których brakuje
	for (auto& twor : wszystkie_obiekty)
	{
		if (!wyglad_tworow.count(twor))
		{
			// brakuje wiêc utworz nowy obiekt do wyœwietlania
			sf::RectangleShape kolo(sf::Vector2f(twor->rozmiar, twor->rozmiar));
		}
	}

	// wygl¹d tworów zawiera dok³adnie to co chcemy wyœwietliæ, uaktualnijmy ich stan
	for (auto& twor : wszystkie_obiekty)
	{
		auto wyglad = wyglad_tworow[twor];
		wyglad.setPosition(twor->polozenie.x, twor->polozenie.y);
		int wysokosc = twor->rozmiar * 640 / 400;  // trzeba to gdzieœ potem wyci¹gnaæ
		wyglad.setSize(sf::Vector2f(twor->rozmiar * 2, wysokosc * 2));
		wyglad.setOrigin(twor->rozmiar, wysokosc);
		wyglad.setFillColor(twor->gracz->kolor);
		if (twor->wyglad == Wyglad::kDomek)
		{
			int ramka_numer = ((clock() * 12 / CLOCKS_PER_SEC)) % 6;
			int ramka = 3 - abs(ramka_numer - 3);
			wyglad.setTexture(obrazek_tworow[twor->wyglad]);
			wyglad.setTextureRect({ 400 * ramka, 0, 400, 640 });
		}
		else if (twor->wyglad == Wyglad::kLudek)
		{
			int ramka_numer = ((clock() * 6 / CLOCKS_PER_SEC)) % 2;
			int ramka = 1 - abs(ramka_numer - 1);
			wyglad.setTexture(obrazek_tworow[twor->wyglad]);
			wyglad.setTextureRect({ 400 * ramka, 0, 400, 640 });

			wyglad.setTexture(obrazek_tworow[twor->wyglad]);
		}

		sf::Text podpis;
		int liczba = 0;
		if (IsType<Domek>(twor))
			liczba = ((Domek*)twor)->liczebnosc;
		else if (IsType<Ludek>(twor))
			liczba = ((Ludek*)twor)->liczebnosc;
		podpis.setFont(czcionka);
		podpis.setCharacterSize(18);
		podpis.setString(std::to_string(liczba));
		podpis.setStyle(sf::Text::Bold);
		podpis.setOutlineColor(sf::Color::Black);
		podpis.setOutlineThickness(2);
		podpis.setFillColor(twor->gracz->kolor);
		podpis.move(twor->polozenie.x - 15 * podpis.getString().getSize() / 2, twor->polozenie.y + wysokosc);

		okno.draw(podpis);
		okno.draw(wyglad);
	}
}

OznaczaczWyborow::OznaczaczWyborow(MyszDecydent & decydent) : decydent(decydent)
{
}

void OznaczaczWyborow::Wyswietlaj(sf::RenderWindow & okno)
{
	if (decydent.wybrany != nullptr)
	{
		double rozmiar = decydent.wybrany->rozmiar * 1.6;
		sf::CircleShape kolo(rozmiar);
		kolo.setPosition(decydent.wybrany->polozenie.x, decydent.wybrany->polozenie.y);
		kolo.setRadius(rozmiar);
		kolo.setOrigin(rozmiar, rozmiar);
		sf::Color kolor = decydent.wybrany->gracz->kolor;
		kolor.a = 128;
		kolo.setFillColor(kolor);

		okno.draw(kolo);
	}
}
