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

Animation Wyswietlacz::ZaladujAnimacje(string& sciezka)
{
	Animation res;
	auto tekstura = new sf::Texture();
	tekstura->loadFromFile(sciezka);
	tekstura->setSmooth(true);

	res.setSpriteSheet(*tekstura);

	int dlugosc_klatki = 400;
	if (abs((int)tekstura->getSize().x % dlugosc_klatki) > 100)
		dlugosc_klatki = 600;

	int klatek = (tekstura->getSize().x + dlugosc_klatki / 2) / dlugosc_klatki;
	for(int i = 0; i < klatek; i++) // TODO trzeba kiedyœ nauczyæ siê czytaæ te¿ w pionie
		res.addFrame(sf::IntRect(dlugosc_klatki * i, 0, dlugosc_klatki, tekstura->getSize().y));

	return res;
}

void Wyswietlacz::Zaladuj(string wybrana_skora)
{
	skorka = wybrana_skora;
	obrazek_tworow[Wyglad::kDomek] = ZaladujAnimacje("Grafika\\" + skorka + "\\kamienica.png");
	obrazek_tworow[Wyglad::kLudek] = ZaladujAnimacje("Grafika\\" + skorka + "\\krasnal.png");

	obrazek_tworow[Wyglad::kUlepszacz] = ZaladujAnimacje("Grafika\\" + skorka + "\\kuznia.png");
	obrazek_tworow[Wyglad::kObrona] = ZaladujAnimacje("Grafika\\" + skorka + "\\zamek.png");

	obrazek_tla.loadFromFile("Grafika\\" + skorka + "\\bruk.png");
	obrazek_tla.setRepeated(true);
}

void Wyswietlacz::ZaladujInne()
{
	this->Zaladuj(next_folder_within_folder("Grafika\\" + skorka));
}

void Wyswietlacz::WyswietlTlo(sf::RenderWindow& okno)
{
	sf::Sprite wyglad_tlo(obrazek_tla);
	wyglad_tlo.setTextureRect({ 0, 0, 1600, 900 });
	okno.draw(wyglad_tlo);

	// namaluj drogi
	for (auto dom : rozgrywka.domki)
	{
		for (auto dokad : dom.drogi)
		{
			sf::Vertex linia[] =
			{
				sf::Vertex(sf::Vector2f(dom.polozenie.x, dom.polozenie.y), sf::Color::Black),
				sf::Vertex(sf::Vector2f(dokad->polozenie.x, dokad->polozenie.y), sf::Color::Black)
			};

			okno.draw(linia, 2, sf::Lines);
		}
	}
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

	// uaktualnij wygl¹dy domków
	for (auto& dom : rozgrywka.domki)
	{
		if (dom.typdomku == TypDomku::kKuznia)
			dom.wyglad = Wyglad::kUlepszacz;
		else if (dom.typdomku == TypDomku::kOsada)
			dom.wyglad = Wyglad::kDomek;
		else if (dom.typdomku == TypDomku::kZamek)
			dom.wyglad = Wyglad::kObrona;
	}

	// wygl¹d tworów zawiera dok³adnie to co chcemy wyœwietliæ, uaktualnijmy ich stan
	for (auto& twor : wszystkie_obiekty)
	{
		auto wyglad = wyglad_tworow[twor];
		wyglad.setPosition(twor->polozenie.x, twor->polozenie.y);
		int wysokosc = twor->rozmiar * 640 / 400;  // trzeba to gdzieœ potem wyci¹gnaæ
		wyglad.setSize(sf::Vector2f(twor->rozmiar * 2, wysokosc * 2));
		wyglad.setOrigin(twor->rozmiar, wysokosc);
		//sf::Color polprzezroczysty = twor->gracz->kolor;
		//polprzezroczysty.a = 128;
		//wyglad.setFillColor(polprzezroczysty);

 		int ziarno = ((unsigned int)twor) % 100;
		int liczba_ramek = obrazek_tworow[twor->wyglad].getSize();
		if (liczba_ramek > 0)
		{
			int ramka_numer = ((clock() * liczba_ramek * 2 / CLOCKS_PER_SEC) + ziarno) % liczba_ramek;
			int ramka = liczba_ramek / 2 - abs(ramka_numer - liczba_ramek / 2);
			wyglad.setTexture(obrazek_tworow[twor->wyglad].getSpriteSheet());
			wyglad.setTextureRect(obrazek_tworow[twor->wyglad].getFrame(ramka));
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
