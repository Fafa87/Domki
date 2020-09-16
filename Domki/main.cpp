#include <thread>

#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <iostream>
#include "easylogging++.h"
INITIALIZE_EASYLOGGINGPP

#include "misja.h"
#include "gui.h"
#include "os.h"
#include "kampania.h"
#include "gui_okna.h"

#include "multi.h"
#include "multi_dzialacze.h"
#include "razem.h"

std::shared_ptr<sfg::Window> kampania_menu(sf::Music& muzyka, string grupa, double poziom, double szybkosc, bool walka_w_polu)
{
    Kampania kampania("Kampanie\\"+grupa);
    //kampania.akt_misja = 7; do testow
    while (kampania.akt_misja <= kampania.lista_misji.size())
    {
        auto misja_dane = kampania.PobierzMisje(kampania.akt_misja);
        auto opis = kampania.PobierzOpis(kampania.akt_misja);
        string przemowa_sciezka = kampania.PobierzOdprawe(kampania.akt_misja);
        sf::Music przemowa;

           misja_dane.trudnosc = poziom;
        misja_dane.szybkosc = szybkosc;
        misja_dane.walka_w_polu=walka_w_polu;


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

        GUI::aplikacja().show_and_wait_for_anything(okno_opisu);

        przemowa.stop();

        // odpal misje
        GUI::aplikacja().hide_all_windows();

        while (misja_dane.Zwyciezca() != 1)
        {
            misja(misja_dane);

            if (misja_dane.Zwyciezca() == 0)
            {
                GUI::aplikacja().show_all_windows();
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

    auto gratulacje = sfg::Label::Create(L"Wygra³es kampaniê!\nGratulacje!");

    auto powrot = sfg::Button::Create(L"Powrót");
    powrot->GetSignal(sfg::Widget::OnLeftClick).Connect(
        [okno] {
        GUI::aplikacja().pop_active_window(okno);
    });

    box->Pack(tytul);
    box->Pack(gratulacje);
    box->Pack(powrot);

    okno->Add(box);

    GUI::aplikacja().show_all_windows();
    return okno;
}

std::shared_ptr<sfg::Window> pojedynczy_gracz_menu(std::shared_ptr<sfg::Window> glowne, sf::Music& muzyka)
{
    auto okno = sfg::Window::Create(sfg::Window::Style::BACKGROUND | sfg::Window::Style::SHADOW);

    okno->SetRequisition(sf::Vector2f(glowne->GetAllocation().width, 0));
    okno->SetAllocation(glowne->GetAllocation());

    auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
    box->SetSpacing(10);
    auto tytul = sfg::Label::Create(WERSJA);
    tytul->SetId("Naglowek");

    box->Pack(tytul, false, false);

    auto kontrolki = WyborMisjiKontrolki::DlaMisji(
        false, // ile ludzi
        true, true, true);
    kontrolki->DodajZestaw(box);

    auto uruchom = sfg::Button::Create("Uruchom");
    uruchom->GetSignal(sfg::Widget::OnLeftClick).Connect(
        [kontrolki, okno, &muzyka] {
        MisjaUstawienia ustawienia;
        ustawienia.nazwa = kontrolki->MisjaNazwa();
        ustawienia.grupa = "Plansza\\" + kontrolki->MisjaGrupa();
        ustawienia.szybkosc = kontrolki->Szybkosc();
        ustawienia.trudnosc = kontrolki->PoziomTrudnosci();
        ustawienia.oszustwa = kontrolki->Oszustwa();
        ustawienia.walka_w_polu = kontrolki->WalkaWPolu(); 
        ustawienia.punkty_kontrolne = kontrolki->PunktyKontrolne();
        ustawienia.do_ilu_wygranych = kontrolki->DoIluWygranych();

        muzyka.stop();
        GUI::aplikacja().hide_all_windows();
        while (ustawienia.Zwyciezca() == -1)
        {
            misja(ustawienia);
        }
        muzyka.play();
        GUI::aplikacja().set_active_window(okno);
    });
    
    auto powrot = sfg::Button::Create(L"Powrót");
    powrot->GetSignal(sfg::Widget::OnLeftClick).Connect(
        [okno] {
        GUI::aplikacja().pop_active_window(okno);
    });

    box->Pack(uruchom, false, false);
    box->Pack(powrot, false, false);
    okno->Add(box);

    GUI::aplikacja().stretch_up_down(okno);

    return okno;
}

std::shared_ptr<sfg::Window> kampania_grand_menu(std::shared_ptr<sfg::Window> glowne, sf::Music& muzyka)
{
    auto okno = sfg::Window::Create(sfg::Window::Style::BACKGROUND | sfg::Window::Style::SHADOW);

    okno->SetRequisition(sf::Vector2f(glowne->GetAllocation().width, 0));
    okno->SetAllocation(glowne->GetAllocation());

    auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
    box->SetSpacing(10);
    auto tytul = sfg::Label::Create(WERSJA);
    tytul->SetId("Naglowek");

    box->Pack(tytul, false, false);

    auto kontrolki = WyborMisjiKontrolki::DlaKampanii();
    kontrolki->DodajZestaw(box);

    auto uruchom = sfg::Button::Create("Uruchom");
    uruchom->GetSignal(sfg::Widget::OnLeftClick).Connect(
        [kontrolki, okno, &muzyka] {
        GUI::aplikacja().hide_all_windows();
        auto okno_kampania = kampania_menu(muzyka, kontrolki->KampaniaNazwa(), kontrolki->PoziomTrudnosci(), kontrolki->Szybkosc(), kontrolki->WalkaWPolu());
        GUI::aplikacja().set_active_window(okno_kampania);
    });

    auto powrot = sfg::Button::Create(L"Powrót");
    powrot->GetSignal(sfg::Widget::OnLeftClick).Connect(
        [okno] {
        GUI::aplikacja().pop_active_window(okno);
    });

    box->Pack(uruchom, false, false);
    box->Pack(powrot, false, false);
    okno->Add(box);

    GUI::aplikacja().stretch_up_down(okno);

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

    auto kampania = sfg::Button::Create("Kampania");
    kampania->GetSignal(sfg::Widget::OnLeftClick).Connect(
        [&muzyka, okno]
    {
        auto okno_kampania = kampania_grand_menu(okno, muzyka);
        GUI::aplikacja().set_active_window(okno_kampania);
    });

    auto pojedynczy = sfg::Button::Create("Sam");
    pojedynczy->GetSignal(sfg::Widget::OnLeftClick).Connect(
        [&muzyka, okno]
    {
        auto okno_sam = pojedynczy_gracz_menu(okno, muzyka);
        GUI::aplikacja().set_active_window(okno_sam);
    });

    auto razem = sfg::Button::Create("Razem");
    razem->GetSignal(sfg::Widget::OnLeftClick).Connect(
        [&muzyka, okno]
    {
        auto okno_razem = wielu_graczy_menu(okno, muzyka);
        GUI::aplikacja().set_active_window(okno_razem);
    });

    tabelka->SetRowSpacings(10);
    tabelka->Attach(kampania, sf::Rect<sf::Uint32>(1, 2, 12, 1), 3, sfg::Table::FILL);
    tabelka->Attach(pojedynczy, sf::Rect<sf::Uint32>(1, 6, 12, 1), 3, sfg::Table::FILL);
    tabelka->Attach(razem, sf::Rect<sf::Uint32>(1, 8, 12, 1), 3, sfg::Table::FILL);

    box->Pack(tytul, false, false);
    box->Pack(tabelka, true, true);
    okno->Add(box);

    return okno;
}

int main() {
    el::Configurations conf("Domki_log.conf");
    el::Loggers::reconfigureAllLoggers(conf);
    LOG(INFO) << "Domki odpalone!";

    sf::Music muzyka;
    
    sf::Texture backtexture;
    backtexture.loadFromFile("Grafika\\houseofhouses2.png");
    backtexture.setRepeated(false);
    backtexture.setSmooth(true);
    sf::Sprite background(backtexture);
    background.setScale(0.45, 0.45);

    auto& okno_menu = GUI::aplikacja().okno;
    GUI::aplikacja().setup_theme(); // <- 74 MB
    
    auto okno = grand_menu(muzyka); // 148 MB

    GUI::aplikacja().put_right_to(okno, background.getGlobalBounds().width);
    GUI::aplikacja().stretch_up_down(okno);

    GUI::aplikacja().set_active_window(okno);

    sf::Event event;
    sf::Clock clock;

    if (!GUI::aplikacja().ini.GetBoolean("przelaczniki", "muzyka", true))
        GUI::aplikacja().dzwieki_glosnosc = 0;

    if (GUI::aplikacja().ini.GetBoolean("przelaczniki", "muzyka", true) && muzyka.openFromFile("Muzyka\\Tytulowa.ogg"))
    {
        muzyka.setVolume(GUI::aplikacja().dzwieki_glosnosc / 3);
        muzyka.play();
    }

    try {
        while (okno_menu.isOpen()) {
            while (okno_menu.pollEvent(event))
            {
                GUI::aplikacja().pulpit.HandleEvent(event);

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
                    GUI::aplikacja().put_right_to(okno, background.getGlobalBounds().width);
                    GUI::aplikacja().stretch_up_down(okno);
                }
            }

            GUI::aplikacja().pulpit.Update(clock.restart().asSeconds()); // 466 MB

            okno_menu.clear();
            okno_menu.draw(background);
            GUI::aplikacja().render();
        }
    }
    catch (const std::exception& ex)
    {
        LOG(ERROR) << ex.what();
        return -1;
    }

    return 0;
}
