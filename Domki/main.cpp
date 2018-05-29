#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include <SFML/Graphics.hpp>

#include "misja.h"
#include "os.h"

vector<string> wczytaj_liste_plansz()
{
	return get_all_names_within_folder("Plansza");
}


void start_serwer(sfg::Desktop& pulpit)
{
	auto okno = sfg::Window::Create(sfg::Window::Style::TOPLEVEL);
	okno->SetRequisition(sf::Vector2f(300, 300));
	okno->SetPosition(sf::Vector2f(150, 50));

	auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 10.0f);
	auto tytul = sfg::Label::Create("SERWER MULTI");

	auto scrolledwindow = sfg::ScrolledWindow::Create();
	scrolledwindow->SetScrollbarPolicy(sfg::ScrolledWindow::HORIZONTAL_AUTOMATIC | sfg::ScrolledWindow::VERTICAL_AUTOMATIC);

	auto historia = sfg::Label::Create("");
	historia->SetRequisition(sf::Vector2f(100.f, 100.f));
	auto tekst = sfg::Entry::Create("");
	
	auto wyslij = sfg::Button::Create("Wyslij");
	wyslij->GetSignal(sfg::Widget::OnLeftClick).Connect([tekst, historia, scrolledwindow] {
		historia->SetText(historia->GetText() + tekst->GetText() + "\n");
		tekst->SetText("");
	});

	wyslij->GetSignal(sfg::Widget::OnExpose).Connect([tekst, historia, scrolledwindow] {
		historia->SetText(historia->GetText() + tekst->GetText() + "\n");
		tekst->SetText("");
	});

	box->Pack(tytul, false);
	scrolledwindow->AddWithViewport(historia);
	scrolledwindow->SetRequisition(sf::Vector2f(100.f, 100.f));
	box->Pack(scrolledwindow, true);
	box->Pack(tekst, false);
	box->Pack(wyslij, false);
	okno->Add(box);

	pulpit.Add(okno);
}

void start_klient(sfg::Desktop& pulpit)
{
	auto okno = sfg::Window::Create(sfg::Window::Style::TOPLEVEL);
	okno->SetRequisition(sf::Vector2f(300, 300));
	okno->SetPosition(sf::Vector2f(150, 50));

	auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 50.0f);
	auto tytul = sfg::Label::Create("KLIENT MULTI");
	auto tabelka = sfg::Table::Create();

	box->Pack(tytul, false);
	box->Pack(tabelka, false);
	okno->Add(box);

	pulpit.Add(okno);
}

int main() {
	sfg::SFGUI sfgui;

	sf::RenderWindow okno_menu(sf::VideoMode(1600, 900), "Domki menu!", sf::Style::None);
	okno_menu.resetGLStates();

	sf::Texture backtexture;
	backtexture.loadFromFile("Grafika\\houseofhouses.png");
	backtexture.setRepeated(false);
	sf::Sprite background(backtexture);
	background.setTextureRect({ 0, 0, 1600, 900 });

	sfg::Desktop pulpit;
	pulpit.LoadThemeFromFile("Grafika\\bazowy.theme");

	auto okno = sfg::Window::Create(sfg::Window::Style::BACKGROUND | sfg::Window::Style::SHADOW);
	pulpit.SetProperty("Window", "BackgroundColor", sf::Color(6, 4, 50));
	pulpit.SetProperty("Label", "Color", sf::Color::White);
	pulpit.SetProperty("Label", "FontSize", 28);
	pulpit.SetProperty("Label#Naglowek", "FontSize", 48);
	pulpit.SetProperty("ComboBox", "BackgroundColor", sf::Color(0, 50, 120));
	pulpit.SetProperty("Button", "BackgroundColor", sf::Color(0, 50, 120));

	okno->SetRequisition(sf::Vector2f(600, 900));
	okno->SetPosition(sf::Vector2f(1000, 0));

	auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 50.0f);
	auto tytul = sfg::Label::Create("DOMKI 0.2");
	tytul->SetId("Naglowek");
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

	auto walka_w_polu_ptaszek = sfg::CheckButton::Create("Walka w polu: ");
	walka_w_polu_ptaszek->SetActive(true);

	auto uruchom = sfg::Button::Create("Uruchom");
	uruchom->GetSignal(sfg::Widget::OnLeftClick).Connect([&] {
		MisjaUstawienia ustawienia;
		ustawienia.nazwa = wybor_lista->GetSelectedText();
		ustawienia.szybkosc = szybkosc_pasek->GetValue();
		ustawienia.trudnosc = trudnosc_lista->GetSelectedText();
		ustawienia.walka_w_polu = walka_w_polu_ptaszek->IsActive();
		misja(ustawienia);
	});

	// multi
	auto serwer = sfg::Button::Create("Serwer");
	serwer->GetSignal(sfg::Widget::OnLeftClick).Connect([&] {
		start_serwer(pulpit);
	});

	auto klient = sfg::Button::Create("Klient");
	klient->GetSignal(sfg::Widget::OnLeftClick).Connect([&] {
		start_klient(pulpit);
	});

	tabelka->Attach(separator, sf::Rect<sf::Uint32>(0, 0, 1, 1), 0, sfg::Table::FILL, sf::Vector2f(20.f, 10.f));
	tabelka->Attach(wybor_etykieta, sf::Rect<sf::Uint32>(1, 0, 1, 1), 0, sfg::Table::FILL, sf::Vector2f(20.f, 10.f));
	tabelka->Attach(wybor_lista, sf::Rect<sf::Uint32>(1, 1, 2, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL);
	tabelka->Attach(trudnosc_etykieta, sf::Rect<sf::Uint32>(1, 2, 1, 1), 0, sfg::Table::FILL, sf::Vector2f(20.f, 10.f));
	tabelka->Attach(trudnosc_lista, sf::Rect<sf::Uint32>(1, 3, 2, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL);
	tabelka->Attach(szybkosc_etykieta, sf::Rect<sf::Uint32>(1, 4, 1, 1), 0, sfg::Table::FILL, sf::Vector2f(20.f, 10.f));
	tabelka->Attach(szybkosc_pasek, sf::Rect<sf::Uint32>(1, 5, 2, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL, sf::Vector2f(20.f, 10.f));
	tabelka->Attach(walka_w_polu_ptaszek, sf::Rect<sf::Uint32>(1, 6, 2, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL, sf::Vector2f(20.f, 10.f));
	tabelka->Attach(separator, sf::Rect<sf::Uint32>(1, 7, 2, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL, sf::Vector2f(20.f, 20.f));
	tabelka->Attach(uruchom, sf::Rect<sf::Uint32>(1, 8, 2, 1), sfg::Table::FILL, sfg::Table::FILL, sf::Vector2f(20.f, 10.f));
	tabelka->Attach(separator, sf::Rect<sf::Uint32>(3, 0, 1, 1), 0, sfg::Table::FILL, sf::Vector2f(20.f, 10.f));
	//tabelka->Attach(serwer, sf::Rect<sf::Uint32>(0, 8, 1, 1), sfg::Table::FILL, sfg::Table::FILL, sf::Vector2f(10.f, 10.f));
	//tabelka->Attach(klient, sf::Rect<sf::Uint32>(1, 8, 1, 1), sfg::Table::FILL, sfg::Table::FILL, sf::Vector2f(10.f, 10.f));

	box->Pack(tytul, false);
	box->Pack(tabelka, false);
	okno->Add(box);

	pulpit.Add(okno);

	sf::Event event;
	sf::Clock clock;

	while (okno_menu.isOpen()) {
		while (okno_menu.pollEvent(event)) 
		{
			pulpit.HandleEvent(event);

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

		pulpit.Update(clock.restart().asSeconds());

		okno_menu.clear();
		okno_menu.draw(background);
		sfgui.Display(okno_menu);
		okno_menu.display();
	}

	return 0;
}
