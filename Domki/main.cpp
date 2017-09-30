#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include <SFML/Graphics.hpp>

#include "misja.h"
#include "os.h"

vector<string> wczytaj_liste_plansz()
{
	return get_all_names_within_folder("Plansza");
}

int main() {
	sfg::SFGUI sfgui;

	sf::RenderWindow okno_menu(sf::VideoMode(800, 600), "Domki menu!", sf::Style::None);
	okno_menu.resetGLStates();

	sf::Texture backtexture;
	backtexture.loadFromFile("Grafika\\menu_tlo.png");
	backtexture.setRepeated(true);
	sf::Sprite background(backtexture);
	background.setTextureRect({ 0, 0, 800, 600 });

	sfg::Desktop desktop;
	desktop.LoadThemeFromFile("Grafika\\bazowy.theme");

	auto okno = sfg::Window::Create(sfg::Window::Style::BACKGROUND);
	okno->SetRequisition(sf::Vector2f(500,500));
	okno->SetPosition(sf::Vector2f(150, 50));

	auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 50.0f);
	auto tytul = sfg::Label::Create("DOMKI 0.1");
	auto tabelka = sfg::Table::Create();

	auto wybor_etykieta = sfg::Label::Create("Misja: ");
	auto separator = sfg::Label::Create("");
	auto wybor_lista = sfg::ComboBox::Create();
	for (auto l : wczytaj_liste_plansz())
		wybor_lista->AppendItem(l);
	wybor_lista->SelectItem(0);

	auto trudnosc_etykieta = sfg::Label::Create("Poziom: ");
	auto trudnosc_lista = sfg::ComboBox::Create();
	for (auto l : poziomy_trudnosci)
		trudnosc_lista->AppendItem(l);
	trudnosc_lista->SelectItem(0);

	auto szybkosc_etykieta = sfg::Label::Create("Szybkosc: ");
	auto szybkosc_pasek = sfg::Scale::Create(0.3, 4, 0.1);
	szybkosc_pasek->SetValue(1.0);

	auto uruchom = sfg::Button::Create("Uruchom");
	uruchom->GetSignal(sfg::Widget::OnLeftClick).Connect([&] {
		MisjaUstawienia ustawienia;
		ustawienia.nazwa = wybor_lista->GetSelectedText();
		ustawienia.szybkosc = szybkosc_pasek->GetValue();
		ustawienia.trudnosc = trudnosc_lista->GetSelectedText();
		misja(ustawienia);
	});

	tabelka->Attach(wybor_etykieta, sf::Rect<sf::Uint32>(0, 0, 1, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL, sf::Vector2f(10.f, 10.f));
	tabelka->Attach(wybor_lista, sf::Rect<sf::Uint32>(0, 1, 2, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL);
	tabelka->Attach(trudnosc_etykieta, sf::Rect<sf::Uint32>(0, 2, 1, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL, sf::Vector2f(10.f, 10.f));
	tabelka->Attach(trudnosc_lista, sf::Rect<sf::Uint32>(0, 3, 2, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL);
	tabelka->Attach(szybkosc_etykieta, sf::Rect<sf::Uint32>(0, 4, 1, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL, sf::Vector2f(10.f, 10.f));
	tabelka->Attach(szybkosc_pasek, sf::Rect<sf::Uint32>(0, 5, 2, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL, sf::Vector2f(10.f, 10.f));
	tabelka->Attach(separator, sf::Rect<sf::Uint32>(0, 6, 2, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL, sf::Vector2f(10.f, 40.f));
	tabelka->Attach(uruchom, sf::Rect<sf::Uint32>(0, 7, 2, 1), sfg::Table::FILL, sfg::Table::FILL, sf::Vector2f(10.f, 10.f));

	box->Pack(tytul, false);
	box->Pack(tabelka, false);
	okno->Add(box);

	desktop.Add(okno);

	sf::Event event;
	sf::Clock clock;

	while (okno_menu.isOpen()) {
		while (okno_menu.pollEvent(event)) 
		{
			desktop.HandleEvent(event);

			switch (event.type)
			{
				case sf::Event::KeyReleased:
					switch (event.key.code)
					{
					case sf::Keyboard::Escape:
						return 0;
					}
					break;
				case sf::Event::Closed:
					return 0;
			}
		}

		desktop.Update(clock.restart().asSeconds());

		okno_menu.clear();
		okno_menu.draw(background);
		sfgui.Display(okno_menu);
		okno_menu.display();
	}

	return 0;
}
