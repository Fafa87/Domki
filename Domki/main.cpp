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

std::shared_ptr<sfg::Window> generator_menu(std::shared_ptr<sfg::Window> glowne, sf::Music& muzyka)
{
    auto okno = sfg::Window::Create(sfg::Window::Style::BACKGROUND | sfg::Window::Style::SHADOW);

    okno->SetRequisition(sf::Vector2f(glowne->GetAllocation().width, 0));
    okno->SetAllocation(glowne->GetAllocation());

    auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
    box->SetSpacing(10);
    box->Pack(utworz_tytulowy_obraz(), false, false);

    string nazwa, sciezka_gry = map_generator(2, nazwa);
    Rozgrywka gra = zwarcie_rozgrywka(sciezka_gry);
    Wyswietlacz rysownik(gra);

    auto minimapa = sfg::Image::Create(rysownik.StworzMinimape({ 320, 180 }));
    minimapa->SetId("minimapa-menu");
    box->Pack(minimapa, false, false); // tworzymy obraz sf::Image i go wyswietlamy w sfgui


    auto nazwa_mapy = sfg::Label::Create(nazwa);
    nazwa_mapy->SetId("nazwa-mapy");
    auto ile_ludzi_etykieta = sfg::Label::Create("Ile graczy: ");
    auto tabelka = sfg::Table::Create();
    auto ile_ludzi_wartosc = sfg::Label::Create("2");
    auto ile_ludzi_pasek = sfg::Scale::Create(2, 8, 1);
    ile_ludzi_pasek->SetValue(2);
    ile_ludzi_pasek->SetIncrements(1, 2);
    ile_ludzi_pasek->GetAdjustment()->GetSignal(sfg::Adjustment::OnChange).Connect(
        [ile_ludzi_wartosc, ile_ludzi_pasek] {
        ile_ludzi_wartosc->SetText(to_string((int)ile_ludzi_pasek->GetValue()));
    });
    tabelka->Attach(nazwa_mapy, sf::Rect<sf::Uint32>(1, 0, 2, 1));
    tabelka->Attach(ile_ludzi_etykieta, sf::Rect<sf::Uint32>(0, 1, 1, 1));
    tabelka->Attach(ile_ludzi_pasek, sf::Rect<sf::Uint32>(1, 1, 2, 1));
    tabelka->Attach(ile_ludzi_wartosc, sf::Rect<sf::Uint32>(3, 1, 1, 1));
    box->Pack(tabelka, false, false);

    GUI::aplikacja().okno.display();
    //narysuj mape

    auto uruchom = sfg::Button::Create("Uruchom");
    uruchom->GetSignal(sfg::Widget::OnLeftClick).Connect(
        [ile_ludzi_pasek, okno, &muzyka, sciezka_gry] {
        MisjaUstawienia ustawienia = wczytaj_meta(sciezka_gry);

        muzyka.stop();
        GUI::aplikacja().hide_all_windows();
        while (ustawienia.Zwyciezca() == -1)
        {
            misja(ustawienia);
        }
        muzyka.play();
        GUI::aplikacja().set_active_window(okno);
    });

    auto generuj = sfg::Button::Create("Generuj ponownie");
    generuj->GetSignal(sfg::Widget::OnLeftClick).Connect(
        [ile_ludzi_pasek, &muzyka, okno]
    {
        string nazwa, sciezka_gry = map_generator((int)ile_ludzi_pasek->GetValue(), nazwa);
        Rozgrywka gra = zwarcie_rozgrywka(sciezka_gry);
        Wyswietlacz rysownik(gra);

        sf::Image minimapa_obraz = rysownik.StworzMinimape({ 320, 180 });
        std::static_pointer_cast<sfg::Image>(okno->GetWidgetById("minimapa-menu"))->SetImage(minimapa_obraz);

        std::static_pointer_cast<sfg::Label>(okno->GetWidgetById("nazwa-mapy"))->SetText(nazwa);
    });

    auto powrot = sfg::Button::Create(L"Powrót");
    powrot->GetSignal(sfg::Widget::OnLeftClick).Connect(
        [okno] {
        GUI::aplikacja().pop_active_window(okno);
    });
    box->Pack(sfg::Separator::Create(), false, false);

    box->Pack(uruchom, false, false);
    box->Pack(generuj, false, false);
    box->Pack(powrot, false, false);
    okno->Add(box);

    GUI::aplikacja().stretch_up_down(okno);

    return okno;
}

std::shared_ptr<sfg::Window> kampania_menu(sf::Music& muzyka, string grupa, double poziom, double szybkosc, bool walka_w_polu)
{
    Kampania kampania("Kampanie\\"+grupa);
    //kampania.akt_misja = 7; do testow
    bool pomin_przemowy = GUI::aplikacja().ini.GetBoolean("przelaczniki", "pomin_przemowy", true);
    while (kampania.akt_misja <= kampania.lista_misji.size())
    {
        auto misja_dane = kampania.PobierzMisje(kampania.akt_misja);
        auto opis = kampania.PobierzOpis(kampania.akt_misja);
        string przemowa_sciezka = kampania.PobierzOdprawe(kampania.akt_misja);
        sf::Music przemowa;

        misja_dane.trudnosc = poziom;
        misja_dane.szybkosc = szybkosc;
        misja_dane.walka_w_polu = walka_w_polu;


        // pokaz opis
        if(!pomin_przemowy)
        {
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
         }
        else muzyka.stop();
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

    auto gratulacje = sfg::Label::Create(L"Wygrałes kampanię!\nGratulacje!");

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
    box->Pack(utworz_tytulowy_obraz(), false, false);

    auto kontrolki = WyborMisjiKontrolki::DlaMisji(
        false, // ile ludzi
        true, true, true);
    kontrolki->DodajZestaw(box);

    string sciezka_gry = "Plansza\\" + kontrolki->MisjaGrupa() + "\\" + kontrolki->MisjaNazwa();
    Rozgrywka gra = zwarcie_rozgrywka(sciezka_gry);
    Wyswietlacz rysownik(gra);

    sf::Image minimapa_obraz = rysownik.StworzMinimape({ 320, 180 });
    std::static_pointer_cast<sfg::Image>(box->GetWidgetById("minimapa-menu"))->SetImage(minimapa_obraz);

    box->GetWidgetById("misja-nazwa")->GetSignal(sfg::ComboBox::OnSelect).Connect([kontrolki, box]
    {
        string sciezka_gry = "Plansza\\" + kontrolki->MisjaGrupa() + "\\" + kontrolki->MisjaNazwa();
        Rozgrywka gra = zwarcie_rozgrywka(sciezka_gry);
        Wyswietlacz rysownik(gra);

        sf::Image minimapa_obraz = rysownik.StworzMinimape({ 320, 180 });
        std::static_pointer_cast<sfg::Image>(box->GetWidgetById("minimapa-menu"))->SetImage(minimapa_obraz);
    });

    box->GetWidgetById("misja-grupa")->GetSignal(sfg::ComboBox::OnSelect).Connect(
        [kontrolki, box] {
        string sciezka_gry = "Plansza\\" + kontrolki->MisjaGrupa() + "\\" + kontrolki->MisjaNazwa();
        Rozgrywka gra = zwarcie_rozgrywka(sciezka_gry);
        Wyswietlacz rysownik(gra);

        sf::Image minimapa_obraz = rysownik.StworzMinimape({ 320, 180 });
        std::static_pointer_cast<sfg::Image>(box->GetWidgetById("minimapa-menu"))->SetImage(minimapa_obraz);
    });


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
    box->Pack(sfg::Separator::Create(), false, false);
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
    box->Pack(utworz_tytulowy_obraz(), false, false);

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

    box->Pack(sfg::Separator::Create(), false, false);
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

    auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 10.0f);
    auto tabelka = sfg::Table::Create();

    auto generator = sfg::Button::Create("Generator");
    generator->GetSignal(sfg::Widget::OnLeftClick).Connect(
        [&muzyka, okno]
    {
        auto okno_generator = generator_menu(okno, muzyka);
        GUI::aplikacja().set_active_window(okno_generator);
    });

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

    auto instrukcja = sfg::Button::Create("Instrukcja");
    instrukcja->GetSignal(sfg::Widget::OnLeftClick).Connect(
        [&muzyka, okno]
    {
        system("instrukcja.html");
    });

    auto uaktualnij = sfg::Button::Create("Uaktualnij");
    uaktualnij->GetSignal(sfg::Widget::OnLeftClick).Connect(
        [&muzyka, okno]
    {
        system("start powershell -ExecutionPolicy Bypass -F Uaktualnij.ps1");
        return okno;
    });

    tabelka->SetRowSpacings(10);
    tabelka->Attach(generator, sf::Rect<sf::Uint32>(1, 1, 12, 1), 3, sfg::Table::FILL);
    tabelka->Attach(kampania, sf::Rect<sf::Uint32>(1, 2, 12, 1), 3, sfg::Table::FILL);
    tabelka->Attach(pojedynczy, sf::Rect<sf::Uint32>(1, 6, 12, 1), 3, sfg::Table::FILL);
    tabelka->Attach(razem, sf::Rect<sf::Uint32>(1, 8, 12, 1), 3, sfg::Table::FILL);
    tabelka->Attach(instrukcja, sf::Rect<sf::Uint32>(1, 12, 12, 1), 3, sfg::Table::FILL);
    tabelka->Attach(uaktualnij, sf::Rect<sf::Uint32>(1, 13, 12, 1), 3, sfg::Table::FILL);

    box->Pack(utworz_tytulowy_obraz(), false, false);
    box->Pack(utworz_napis_wersji(), false, false);
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

    // ustaw kursor
    sf::Image kursor;
    kursor.loadFromFile("Grafika\\sword2.png");

    sf::Cursor cursor;
    if (cursor.loadFromPixels(kursor.getPixelsPtr(), sf::Vector2u(32,32), sf::Vector2u(0, 0)))
        okno_menu.setMouseCursor(cursor);

    sf::Image ikonka;
    ikonka.loadFromFile("Grafika\\domcraft_small.png");
    okno_menu.setIcon(64, 64, ikonka.getPixelsPtr());

    if (!GUI::aplikacja().ini.GetBoolean("przelaczniki", "muzyka", true))
        GUI::aplikacja().dzwieki_glosnosc = 0;

    if (GUI::aplikacja().ini.GetBoolean("przelaczniki", "muzyka", true) && muzyka.openFromFile("Muzyka\\Tytulowa.ogg"))
    {
        muzyka.setVolume(GUI::aplikacja().dzwieki_glosnosc / 3);
        muzyka.play();
    }

    if (!GUI::aplikacja().ini.GetBoolean("przelaczniki", "pokaz_ranking", true))
        GUI::aplikacja().pokaz_ranking = false;

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
                        //GUI::aplikacja().close_all_windows();
                        return EXIT_SUCCESS;
                    }
                    break;
                case sf::Event::Closed:
                    //GUI::aplikacja().close_all_windows();
                    return EXIT_SUCCESS;
                //case sf::Event::Resized:
                    //GUI::aplikacja().put_right_to(okno, background.getGlobalBounds().width);
                    //GUI::aplikacja().stretch_up_down(okno);
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
