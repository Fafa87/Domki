#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "misja.h"
#include "gui.h"
#include "os.h"
#include "kampania.h"

const string WERSJA = "DOMKI 0.8";

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

std::shared_ptr<sfg::Window> kampania_menu(sf::Music& muzyka, string poziom)
{
	Kampania kampania("Kampania");
	//kampania.akt_misja = 7; do testow
	while (kampania.akt_misja <= kampania.lista_misji.size())
	{
		auto misja_dane = kampania.PobierzMisje(kampania.akt_misja);
		auto opis = kampania.PobierzOpis(kampania.akt_misja);
		string przemowa_sciezka = kampania.PobierzOdprawe(kampania.akt_misja);
		sf::Music przemowa;

		if (poziom.size() != 0)
			misja_dane.trudnosc = poziom;

		// pokaz opis
		auto okno_opisu = sfg::Window::Create(sfg::Window::Style::BACKGROUND | sfg::Window::Style::SHADOW);
		okno_opisu->SetRequisition(sf::Vector2f(900, 600));

		auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 50.0f);

		auto tytul = sfg::Label::Create(opis.powitanie);
		tytul->SetId("Naglowek");
		box->Pack(tytul);

		auto opis_fabuly = sfg::Label::Create(opis.fabula);
		box->Pack(opis_fabuly);

		auto opis_nauka = sfg::Label::Create(opis.nauka);
		box->Pack(opis_nauka);

		okno_opisu->Add(box);
		muzyka.stop();
		if (przemowa.openFromFile(przemowa_sciezka))
			przemowa.play();

		GUI::aplikacja.show_and_wait_for_anything(okno_opisu);

		przemowa.stop();

		// odpal misje
		GUI::aplikacja.hide_all_windows();

		while (misja_dane.Zwyciezca() != 1)
		{
			misja(misja_dane);

			if (misja_dane.Zwyciezca() == 0)
			{
				GUI::aplikacja.show_all_windows();
				return nullptr;
			}
		}
		muzyka.play();

		kampania.akt_misja++;

		// jak porazka to powtorz
	}

	auto okno = sfg::Window::Create(sfg::Window::Style::BACKGROUND | sfg::Window::Style::SHADOW);

	okno->SetRequisition(sf::Vector2f(400, 900));
	okno->SetPosition(sf::Vector2f(880, 0));

	auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 50.0f);
	auto tytul = sfg::Label::Create(kampania.nazwa);
	tytul->SetId("Naglowek");

	auto gratulacje = sfg::Label::Create("Wygrales kampanie!\nGratulacje!");

	auto powrot = sfg::Button::Create("Powrot");
	powrot->GetSignal(sfg::Widget::OnLeftClick).Connect(
		[okno] {
		GUI::aplikacja.pop_active_window(okno);
	});

	box->Pack(tytul);
	box->Pack(gratulacje);
	box->Pack(powrot);

	okno->Add(box);

	GUI::aplikacja.show_all_windows();
	return okno;
}

std::shared_ptr<sfg::Window> pojedynczy_gracz_menu(std::shared_ptr<sfg::Window> glowne, sf::Music& muzyka)
{
	auto okno = sfg::Window::Create(sfg::Window::Style::BACKGROUND | sfg::Window::Style::SHADOW);

	//okno->SetRequisition(sf::Vector2f(480, 0));
	okno->SetRequisition(sf::Vector2f(glowne->GetAllocation().width, 0));
	okno->SetAllocation(glowne->GetAllocation());

	auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
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

	auto do_ilu_etykieta = sfg::Label::Create("Do ilu: ");
	auto do_ilu_wartosc = sfg::Label::Create("1");
	auto do_ilu_pasek = sfg::Scale::Create(1, 6, 1);
	do_ilu_pasek->SetValue(1);
	do_ilu_pasek->SetIncrements(1, 2);
	do_ilu_pasek->GetAdjustment()->GetSignal(sfg::Adjustment::OnChange).Connect(
		[do_ilu_wartosc, do_ilu_pasek] {
		do_ilu_wartosc->SetText(to_string((int)do_ilu_pasek->GetValue()));
	});
	
	auto inne_etykieta = sfg::Label::Create("Inne: ");
	auto walka_w_polu_ptaszek = sfg::CheckButton::Create("Walka w polu: ");
	walka_w_polu_ptaszek->SetActive(true);

	auto uruchom = sfg::Button::Create("Uruchom");
	uruchom->GetSignal(sfg::Widget::OnLeftClick).Connect(
		[wybor_lista, szybkosc_pasek, trudnosc_lista, walka_w_polu_ptaszek, do_ilu_pasek, okno, &muzyka] {
		MisjaUstawienia ustawienia;
		ustawienia.nazwa = wybor_lista->GetSelectedText();
		ustawienia.szybkosc = szybkosc_pasek->GetValue();
		ustawienia.trudnosc = trudnosc_lista->GetSelectedText();
		ustawienia.walka_w_polu = walka_w_polu_ptaszek->IsActive();
		ustawienia.do_ilu_wygranych = do_ilu_pasek->GetValue();

		muzyka.stop();
		GUI::aplikacja.hide_all_windows();
		while (ustawienia.Zwyciezca() == -1)
		{
			misja(ustawienia);
		}
		muzyka.play();
		GUI::aplikacja.set_active_window(okno);
	});
	
	auto powrot = sfg::Button::Create("Powrot");
	powrot->GetSignal(sfg::Widget::OnLeftClick).Connect(
		[okno] {
		GUI::aplikacja.pop_active_window(okno);
	});

	tabelka->SetRowSpacings(10);

	tabelka->Attach(separator, sf::Rect<sf::Uint32>(0, 0, 4, 1));
	tabelka->Attach(wybor_etykieta, sf::Rect<sf::Uint32>(0, 1, 1, 1));
	tabelka->Attach(wybor_lista, sf::Rect<sf::Uint32>(1, 1, 2, 1));
	tabelka->Attach(trudnosc_etykieta, sf::Rect<sf::Uint32>(0, 2, 1, 1));
	tabelka->Attach(trudnosc_lista, sf::Rect<sf::Uint32>(1, 2, 2, 1));

	tabelka->Attach(separator, sf::Rect<sf::Uint32>(0, 3, 4, 1));

	tabelka->Attach(szybkosc_etykieta, sf::Rect<sf::Uint32>(0, 4, 1, 1));
	tabelka->Attach(szybkosc_pasek, sf::Rect<sf::Uint32>(1, 4, 2, 1));
	tabelka->Attach(do_ilu_etykieta, sf::Rect<sf::Uint32>(0, 5, 1, 1));
	tabelka->Attach(do_ilu_pasek, sf::Rect<sf::Uint32>(1, 5, 1, 1));
	tabelka->Attach(do_ilu_wartosc, sf::Rect<sf::Uint32>(2, 5, 1, 1));
	
	tabelka->Attach(inne_etykieta, sf::Rect<sf::Uint32>(0, 6, 1, 1));
	tabelka->Attach(walka_w_polu_ptaszek, sf::Rect<sf::Uint32>(1, 6, 2, 1));
	tabelka->Attach(separator, sf::Rect<sf::Uint32>(0, 7, 4, 1));

	tabelka->Attach(uruchom, sf::Rect<sf::Uint32>(1, 8, 2, 1));
	tabelka->Attach(powrot, sf::Rect<sf::Uint32>(1, 9, 2, 1));

	box->Pack(tytul, false, false);
	box->Pack(tabelka, true, false);
	okno->Add(box);

	GUI::aplikacja.stretch_up_down(okno);

	return okno;
}

std::shared_ptr<sfg::Window> grand_menu(sf::Music& muzyka)
{
	auto okno = sfg::Window::Create(sfg::Window::Style::BACKGROUND | sfg::Window::Style::SHADOW);
	okno->SetRequisition(sf::Vector2f(480, 0));

	auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 100.0f);

	auto tytul = sfg::Label::Create(WERSJA);
	tytul->SetId("Naglowek");
	auto tabelka = sfg::Table::Create();

	auto kampania1 = sfg::Button::Create(L"Kampania �atwa");
	kampania1->GetSignal(sfg::Widget::OnLeftClick).Connect(
		[&muzyka]
	{
		auto okno_kampanii = kampania_menu(muzyka, poziomy_trudnosci[0]);
		if (okno_kampanii != nullptr)
			GUI::aplikacja.set_active_window(okno_kampanii);
	});

	auto kampania2 = sfg::Button::Create("Kampania trudna");
	kampania2->GetSignal(sfg::Widget::OnLeftClick).Connect(
		[&muzyka]
	{
		auto okno_kampanii = kampania_menu(muzyka, poziomy_trudnosci[1]);
		if (okno_kampanii != nullptr)
			GUI::aplikacja.set_active_window(okno_kampanii);
	});

	auto pojedynczy = sfg::Button::Create("Sam");
	pojedynczy->GetSignal(sfg::Widget::OnLeftClick).Connect(
		[&muzyka, okno]
	{
		auto okno_sam = pojedynczy_gracz_menu(okno, muzyka);
		GUI::aplikacja.set_active_window(okno_sam);
	});

	tabelka->SetRowSpacings(10);
	tabelka->Attach(kampania1, sf::Rect<sf::Uint32>(1, 2, 12, 1), 3, sfg::Table::FILL);
	tabelka->Attach(kampania2, sf::Rect<sf::Uint32>(1, 4, 12, 1), 3, sfg::Table::FILL);
	tabelka->Attach(pojedynczy, sf::Rect<sf::Uint32>(1, 6, 12, 1), 3, sfg::Table::FILL);

	box->Pack(tytul, false, false);
	box->Pack(tabelka, true, true);
	okno->Add(box);

	return okno;
}

int main() {
	sf::Music muzyka;

	sf::Texture backtexture;
	backtexture.loadFromFile("Grafika\\houseofhouses2.png");
	backtexture.setRepeated(false);
	backtexture.setSmooth(true);
	sf::Sprite background(backtexture);
	background.setScale(0.45, 0.45);

	auto& okno_menu = GUI::aplikacja.okno;
	GUI::aplikacja.setup_theme();
	
	auto okno = grand_menu(muzyka);

	GUI::aplikacja.put_right_to(okno, background.getGlobalBounds().width);
	GUI::aplikacja.stretch_up_down(okno);

	GUI::aplikacja.set_active_window(okno);

	sf::Event event;
	sf::Clock clock;

	if (muzyka.openFromFile("Muzyka\\Tytulowa.ogg"))
	{	
		muzyka.setVolume(50);
		muzyka.play();
	}

	try {
		while (okno_menu.isOpen()) {
			while (okno_menu.pollEvent(event))
			{
				GUI::aplikacja.pulpit.HandleEvent(event);

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
				case sf::Event::Resized:
					GUI::aplikacja.put_right_to(okno, background.getGlobalBounds().width);
					GUI::aplikacja.stretch_up_down(okno);
				}
			}

			GUI::aplikacja.pulpit.Update(clock.restart().asSeconds());

			okno_menu.clear();
			okno_menu.draw(background);
			GUI::aplikacja.render();
		}
	}
	catch (const std::exception&)
	{
		return -1;
	}

	return 0;
}
