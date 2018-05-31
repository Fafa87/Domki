#include "Narzedzia\AnimatedSprite.hpp"
#include "wyswietlacze.h"

#include<set>
#include<string>
#include<ctime>

#define _USE_MATH_DEFINES
#include<math.h>

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
	tekstura->generateMipmap();

	res.setSpriteSheet(*tekstura);

	int dlugosc_klatki = 400;
	if (abs((int)tekstura->getSize().x % dlugosc_klatki) > 100)
		dlugosc_klatki = 600;

	int klatek = (tekstura->getSize().x + dlugosc_klatki / 2) / dlugosc_klatki;
	for(int i = 0; i < klatek; i++) // TODO trzeba kiedyï¿½ nauczyï¿½ siï¿½ czytaï¿½ teï¿½ w pionie
		res.addFrame(sf::IntRect(dlugosc_klatki * i, 0, dlugosc_klatki, tekstura->getSize().y));

	// wczytaj powrotne
	for (int i = klatek - 1; i >= 1; i--)
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
		for (auto dokad : dom.drogi) if(dokad->uid < dom.uid) // maluj tylko w jedn¹ stronê
		{
			sf::Vertex linia[] =
			{
				sf::Vertex(sf::Vector2f(dom.polozenie.x, dom.polozenie.y + 10), sf::Color::Black),
				sf::Vertex(sf::Vector2f(dokad->polozenie.x, dokad->polozenie.y + 10), sf::Color::Black)
			};
			int odleglosc = sqrt(pow(dokad->polozenie.x - dom.polozenie.x, 2) + pow(dokad->polozenie.y - dom.polozenie.y, 2));
			sf::RectangleShape linijka(sf::Vector2f(odleglosc, 4));
			linijka.setPosition(linia[0].position);

			linijka.setRotation(atan2(linia[1].position.y - linia[0].position.y, linia[1].position.x - linia[0].position.x) / M_PI * 180);
			linijka.setFillColor(sf::Color(150, 75, 0));
			linijka.setOutlineColor(sf::Color::Black);
			linijka.setOutlineThickness(2);

			okno.draw(linijka);
			//okno.draw(linia, 2, sf::Lines);
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

	// usuï¿½ wyglï¿½dy ktï¿½rych juï¿½ nie ma
	vector<Twor*> do_usuniecia;
	for (auto& wyg_map : wyglad_tworow)
		if (!wszystkie_obiekty.count(wyg_map.first))
			do_usuniecia.push_back(wyg_map.first);

	for (auto twor : do_usuniecia)
		wyglad_tworow.erase(twor);

	// dodaj wyglï¿½dy ktï¿½rych brakuje
	for (auto& twor : wszystkie_obiekty)
	{
		if (!wyglad_tworow.count(twor))
		{
			// brakuje wiï¿½c utworz nowy obiekt do wyï¿½wietlania
			sf::RectangleShape kolo(sf::Vector2f(twor->rozmiar, twor->rozmiar));
		}
	}

	// uaktualnij wyglï¿½dy domkï¿½w
	for (auto& dom : rozgrywka.domki)
	{
		if (dom.typdomku == TypDomku::kKuznia)
			dom.wyglad = Wyglad::kUlepszacz;
		else if (dom.typdomku == TypDomku::kOsada)
			dom.wyglad = Wyglad::kDomek;
		else if (dom.typdomku == TypDomku::kZamek)
			dom.wyglad = Wyglad::kObrona;
	}

	// wyglï¿½d tworï¿½w zawiera dokï¿½adnie to co chcemy wyï¿½wietliï¿½, uaktualnijmy ich stan
	for (auto& twor : wszystkie_obiekty)
	{
		auto wyglad = wyglad_tworow[twor];
		wyglad.setPosition(twor->polozenie.x, twor->polozenie.y);

		int liczba_ramek = obrazek_tworow[twor->wyglad].getSize();
		int dlugosc_jednego = 10;
		int wysokosc_jednego = 10;
		if (liczba_ramek > 0)
		{
			dlugosc_jednego = obrazek_tworow[twor->wyglad].getFrame(0).width;
			wysokosc_jednego = obrazek_tworow[twor->wyglad].getFrame(0).height;
		}

		int wysokosc = twor->rozmiar * wysokosc_jednego / dlugosc_jednego;  // trzeba to gdzieï¿½ potem wyciï¿½gnaï¿½
		wyglad.setSize(sf::Vector2f(twor->rozmiar * 2, wysokosc * 2));
		wyglad.setOrigin(twor->rozmiar, wysokosc);
		//sf::Color polprzezroczysty = twor->gracz->kolor;
		//polprzezroczysty.a = 128;
		//wyglad.setFillColor(polprzezroczysty);

		int ziarno = ((unsigned int)twor) % 100;

		if (liczba_ramek > 0)
		{
			int ramka_numer = ((clock() * liczba_ramek * 2 / CLOCKS_PER_SEC) + ziarno) % liczba_ramek;
			wyglad.setTexture(obrazek_tworow[twor->wyglad].getSpriteSheet());
			wyglad.setTextureRect(obrazek_tworow[twor->wyglad].getFrame(ramka_numer));
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

		// rysuj poziom domku
		if (IsType<Domek>(twor))
		{
			int poziom = ((Domek*)twor)->poziom;
			if (poziom > 1)
			{
				podpis.setFillColor(sf::Color::White);
				podpis.setOutlineColor(twor->gracz->kolor);
				podpis.setString(std::to_string(poziom));
				podpis.setPosition(twor->polozenie.x - 15 * podpis.getString().getSize() / 2, twor->polozenie.y - wysokosc * 1.5);
				okno.draw(podpis);
			}
		}
		// rysuj tarcze ludka
		if (IsType<Ludek>(twor))
		{
			int tarcza = ((Ludek*)twor)->tarcza;
			if (tarcza > 0)
			{
				podpis.setFillColor(sf::Color::White);
				podpis.setOutlineColor(twor->gracz->kolor);
				podpis.setString(std::to_string(tarcza));
				podpis.setPosition(twor->polozenie.x - 15 * podpis.getString().getSize() / 2, twor->polozenie.y - wysokosc * 2);
				okno.draw(podpis);
			}

			bool lustro = ((Ludek*)twor)->cel->polozenie.x < ((Ludek*)twor)->polozenie.x;
			if (lustro)
				wyglad.setScale(-1, 1);
		}

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
