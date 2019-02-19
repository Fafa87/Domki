#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "misja.h"
#include "gui.h"
#include "os.h"
#include "kampania.h"

const string WERSJA = "DOMKI 0.6";

void start_serwer(sfg::Desktop& pulpit, sf::Music& muzyka)
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

void start_klient(sfg::Desktop& pulpit, sf::Music& muzyka)
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

std::shared_ptr<sfg::Window> kampania_menu(sfg::Desktop& pulpit, sf::RenderWindow& okno_menu, sf::Music& muzyka)
{
	Kampania kampania("Kampania");

	while (kampania.akt_misja < kampania.lista_misji.size())
	{
		auto misja_dane = kampania.PobierzMisje(kampania.akt_misja);
		auto opis = kampania.PobierzOpis(kampania.akt_misja);

		// pokaz opis
		auto okno_opisu = sfg::Window::Create(sfg::Window::Style::BACKGROUND | sfg::Window::Style::SHADOW);
		okno_opisu->SetRequisition(sf::Vector2f(900, 600));
		GUI::center_window(okno_menu, okno_opisu);

		auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 50.0f);

		auto tytul = sfg::Label::Create(opis.powitanie);
		tytul->SetId("Naglowek");
		box->Pack(tytul);

		auto opis_fabuly = sfg::Label::Create(opis.fabula);
		box->Pack(opis_fabuly);

		auto opis_nauka = sfg::Label::Create(opis.nauka);
		box->Pack(opis_nauka);

		okno_opisu->Add(box);
		pulpit.Add(okno_opisu);

		GUI::pulpit.Update(1);
		GUI::sfgui.Display(okno_menu);
		okno_menu.display();

		GUI::wait_for_anything(okno_menu);

		pulpit.Remove(okno_opisu);
		okno_opisu->Show(false);


		// odpal misje
		muzyka.stop();
		okno_menu.setVisible(false);
		GUI::hide_all_windows();

		while (misja_dane.Zwyciezca() != 1)
		{
			misja(misja_dane);

			if (misja_dane.Zwyciezca() == 0)
				return nullptr;
		}
		muzyka.play();
		okno_menu.setVisible(true);

		kampania.akt_misja++;

		// jak porazka to powtorz
	}

	auto okno = sfg::Window::Create(sfg::Window::Style::BACKGROUND | sfg::Window::Style::SHADOW);

	okno->SetRequisition(sf::Vector2f(600, 900));
	okno->SetPosition(sf::Vector2f(1000, 0));

	auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 50.0f);
	auto tytul = sfg::Label::Create(kampania.nazwa);
	tytul->SetId("Naglowek");

	auto gratulacje = sfg::Label::Create("Wygrales kampanie!\nGratulacje!");

	auto powrot = sfg::Button::Create("Powrot");
	powrot->GetSignal(sfg::Widget::OnLeftClick).Connect(
		[okno, &pulpit, &okno_menu, &muzyka] {
		GUI::remove_active_window(okno);
		okno->Show(false);
	});

	box->Pack(tytul);
	box->Pack(gratulacje);
	box->Pack(powrot);

	okno->Add(box);
	return okno;
}

std::shared_ptr<sfg::Window> pojedynczy_gracz_menu(sfg::Desktop& pulpit, sf::RenderWindow& okno_menu, sf::Music& muzyka)
{
	auto okno = sfg::Window::Create(sfg::Window::Style::BACKGROUND | sfg::Window::Style::SHADOW);

	okno->SetRequisition(sf::Vector2f(600, 900));
	okno->SetPosition(sf::Vector2f(1000, 0));

	auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 50.0f);
	auto tytul = sfg::Label::Create(WERSJA);
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
	szybkosc_pasek->SetValue(1.5);
	szybkosc_pasek->SetRequisition(sf::Vector2f(200.f, 30.f));

	auto do_ilu_etykieta = sfg::Label::Create("Do ilu wygranych: ");
	auto do_ilu_wartosc = sfg::Label::Create("1");
	auto do_ilu_pasek = sfg::Scale::Create(1, 6, 1);
	do_ilu_pasek->SetRequisition(sf::Vector2f(200.f, 30.f));
	do_ilu_pasek->SetValue(1);
	do_ilu_pasek->SetIncrements(1, 2);
	do_ilu_pasek->GetAdjustment()->GetSignal(sfg::Adjustment::OnChange).Connect(
		[do_ilu_wartosc, do_ilu_pasek] {
		do_ilu_wartosc->SetText(to_string((int)do_ilu_pasek->GetValue()));
	});

	auto walka_w_polu_ptaszek = sfg::CheckButton::Create("Walka w polu: ");
	walka_w_polu_ptaszek->SetActive(true);

	auto uruchom = sfg::Button::Create("Uruchom");
	uruchom->GetSignal(sfg::Widget::OnLeftClick).Connect(
		[wybor_lista, szybkosc_pasek, trudnosc_lista, walka_w_polu_ptaszek, do_ilu_pasek, okno, &okno_menu, &muzyka] {
		MisjaUstawienia ustawienia;
		ustawienia.nazwa = wybor_lista->GetSelectedText();
		ustawienia.szybkosc = szybkosc_pasek->GetValue();
		ustawienia.trudnosc = trudnosc_lista->GetSelectedText();
		ustawienia.walka_w_polu = walka_w_polu_ptaszek->IsActive();
		ustawienia.do_ilu_wygranych = do_ilu_pasek->GetValue();

		muzyka.stop();
		okno_menu.setVisible(false);
		okno->Show(false);
		GUI::hide_all_windows();
		while (ustawienia.Zwyciezca() == -1)
		{
			misja(ustawienia);
		}
		muzyka.play();
		okno->Show(true);
		okno_menu.setVisible(true);
	});
	
	auto powrot = sfg::Button::Create("Powrot");
	powrot->GetSignal(sfg::Widget::OnLeftClick).Connect(
		[okno, &pulpit, &okno_menu, &muzyka] {
		GUI::remove_active_window(okno);
		okno->Show(false);
	});

	tabelka->SetRowSpacings(10);
	tabelka->Attach(separator, sf::Rect<sf::Uint32>(0, 0, 1, 1), 0, sfg::Table::FILL, sf::Vector2f(20.f, 10.f));
	tabelka->Attach(wybor_etykieta, sf::Rect<sf::Uint32>(1, 0, 1, 1), 0, sfg::Table::FILL, sf::Vector2f(20.f, 10.f));
	tabelka->Attach(wybor_lista, sf::Rect<sf::Uint32>(1, 1, 3, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL);
	tabelka->Attach(trudnosc_etykieta, sf::Rect<sf::Uint32>(1, 2, 1, 1), 0, sfg::Table::FILL, sf::Vector2f(20.f, 10.f));
	tabelka->Attach(trudnosc_lista, sf::Rect<sf::Uint32>(1, 3, 3, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL);

	tabelka->Attach(szybkosc_etykieta, sf::Rect<sf::Uint32>(1, 4, 1, 1), 0, 0, sf::Vector2f(20.f, 10.f));
	tabelka->Attach(szybkosc_pasek, sf::Rect<sf::Uint32>(1, 5, 3, 1), sfg::Table::FILL, 0, sf::Vector2f(10.f, 5.f));
	tabelka->Attach(do_ilu_etykieta, sf::Rect<sf::Uint32>(1, 6, 1, 1), 0, 0, sf::Vector2f(20.f, 10.f));
	tabelka->Attach(do_ilu_pasek, sf::Rect<sf::Uint32>(1, 7, 2, 1), sfg::Table::FILL, 0, sf::Vector2f(20.f, 5.f));
	tabelka->Attach(do_ilu_wartosc, sf::Rect<sf::Uint32>(3, 7, 1, 1), sfg::Table::FILL, 0, sf::Vector2f(20.f, 5.f));

	tabelka->Attach(walka_w_polu_ptaszek, sf::Rect<sf::Uint32>(1, 8, 3, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL, sf::Vector2f(20.f, 10.f));
	tabelka->Attach(separator, sf::Rect<sf::Uint32>(1, 9, 3, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL, sf::Vector2f(20.f, 20.f));
	tabelka->Attach(uruchom, sf::Rect<sf::Uint32>(1, 10, 3, 1), sfg::Table::FILL, sfg::Table::FILL, sf::Vector2f(20.f, 10.f));
	tabelka->Attach(separator, sf::Rect<sf::Uint32>(3, 0, 1, 1), 0, sfg::Table::FILL, sf::Vector2f(20.f, 10.f));

	tabelka->Attach(separator, sf::Rect<sf::Uint32>(1, 11, 3, 1), 0, sfg::Table::FILL, sf::Vector2f(20.f, 10.f));
	tabelka->Attach(powrot, sf::Rect<sf::Uint32>(1, 13, 3, 1), sfg::Table::FILL, sfg::Table::FILL, sf::Vector2f(20.f, 10.f));
	//tabelka->Attach(serwer, sf::Rect<sf::Uint32>(0, 8, 1, 1), sfg::Table::FILL, sfg::Table::FILL, sf::Vector2f(10.f, 10.f));
	//tabelka->Attach(klient, sf::Rect<sf::Uint32>(1, 8, 1, 1), sfg::Table::FILL, sfg::Table::FILL, sf::Vector2f(10.f, 10.f));

	box->Pack(tytul, false, false);
	box->Pack(tabelka, false, false);
	okno->Add(box);

	return okno;
}

std::shared_ptr<sfg::Window> grand_menu(sfg::Desktop& pulpit, sf::RenderWindow& okno_menu, sf::Music& muzyka)
{
	auto okno = sfg::Window::Create(sfg::Window::Style::BACKGROUND | sfg::Window::Style::SHADOW);

	okno->SetRequisition(sf::Vector2f(600, 900));
	okno->SetPosition(sf::Vector2f(1000, 0));

	auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 100.0f);
	box->SetRequisition(sf::Vector2f(600, 0));

	auto tytul = sfg::Label::Create(WERSJA);
	tytul->SetId("Naglowek");
	auto tabelka = sfg::Table::Create();

	auto kampania = sfg::Button::Create("Kampania");
	kampania->SetRequisition(sf::Vector2f(400, 80));
	kampania->GetSignal(sfg::Widget::OnLeftClick).Connect(
		[&pulpit, &okno_menu, &muzyka]
	{
		auto okno_kampanii = kampania_menu(pulpit, okno_menu, muzyka);
		GUI::set_active_window(okno_kampanii);
		pulpit.Add(okno_kampanii);
	});

	auto pojedynczy = sfg::Button::Create("Sam");
	pojedynczy->SetRequisition(sf::Vector2f(400, 80));
	pojedynczy->GetSignal(sfg::Widget::OnLeftClick).Connect(
		[&pulpit, &okno_menu, &muzyka]
	{
		auto okno_sam = pojedynczy_gracz_menu(pulpit, okno_menu, muzyka);
		GUI::set_active_window(okno_sam);
		pulpit.Add(okno_sam);
	});

	tabelka->SetRowSpacings(10);
	tabelka->SetRequisition(sf::Vector2f(400, 0));
	tabelka->Attach(kampania, sf::Rect<sf::Uint32>(1, 2, 12, 1), sfg::Table::FILL, sfg::Table::FILL);
	tabelka->Attach(pojedynczy, sf::Rect<sf::Uint32>(1, 4, 12, 1), sfg::Table::FILL, sfg::Table::FILL);

	box->Pack(tytul, false, false);
	box->Pack(tabelka, true, true);
	okno->Add(box);

	return okno;
}

int main() {
	sf::RenderWindow okno_menu(sf::VideoMode(1600, 900), "Domki menu!", sf::Style::None);
	okno_menu.resetGLStates();

	sf::Music muzyka;

	sf::Texture backtexture;
	backtexture.loadFromFile("Grafika\\houseofhouses.png");
	backtexture.setRepeated(false);
	sf::Sprite background(backtexture);
	background.setTextureRect({ 0, 0, 1600, 900 });

	GUI::setup_theme();

	//// multi
	//auto serwer = sfg::Button::Create("Serwer");
	//serwer->GetSignal(sfg::Widget::OnLeftClick).Connect([&] {
	//	start_serwer(GUI::pulpit, muzyka);
	//});

	//auto klient = sfg::Button::Create("Klient");
	//klient->GetSignal(sfg::Widget::OnLeftClick).Connect([&] {
	//	start_klient(GUI::pulpit, muzyka);
	//});

	
	auto okno = grand_menu(GUI::pulpit, okno_menu, muzyka);
	GUI::set_active_window(okno);
	GUI::pulpit.Add(okno);

	sf::Event event;
	sf::Clock clock;

	if (muzyka.openFromFile("Muzyka\\Tytulowa.ogg"))
	{	
		muzyka.setVolume(50);
		muzyka.play();
	}

	while (okno_menu.isOpen()) {
		while (okno_menu.pollEvent(event)) 
		{
			GUI::pulpit.HandleEvent(event);

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

		GUI::pulpit.Update(clock.restart().asSeconds());

		okno_menu.clear();
		okno_menu.draw(background);
		GUI::sfgui.Display(okno_menu);
		okno_menu.display();
	}

	return 0;
}
