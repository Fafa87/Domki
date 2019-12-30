#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include <SFML/Graphics.hpp>

#include "easylogging++.h"
INITIALIZE_EASYLOGGINGPP

#include "../Domki/gui.h"
#include "../Domki/misja.h"
#include "../Domki/os.h"


using namespace std;

#include <iterator>

void dodaj_gracza(Rozgrywka & gra, Gracz& g, double x, double y)
{
    gra.domki.push_back(Domek());
    Domek& domek = gra.domki.back();
    domek.polozenie = { x,y };
    domek.produkcja = 2;
    domek.max_liczebnosc = 100;
    domek.wyglad = Wyglad::kMiasto;
    domek.gracz = &g;
    domek.poziom = 1;
    gra.ZmienLiczebnosc(domek, 50);

    gra.domki.push_back(Domek());
    Domek& domek2 = gra.domki.back();
    domek2.polozenie = { x,y + 150 };
    domek2.produkcja = 2;
    domek2.max_liczebnosc = 100;
    domek2.wyglad = Wyglad::kMiasto;
    domek2.gracz = &g;
    domek2.poziom = 10;
    gra.ZmienLiczebnosc(domek2, 250);
    domek2.drogi.push_back(&domek);

    gra.domki.push_back(Domek());
    Domek& domek3 = gra.domki.back();
    domek3.polozenie = { x - 100 ,y + 150 };
    domek3.typdomku = TypDomku::kZbrojownia;
    domek3.gracz = &g;
    domek3.poziom = 1;
    gra.ZmienLiczebnosc(domek3, 50);
    domek3.drogi.push_back(&domek);

    gra.domki.push_back(Domek());
    Domek& domek4 = gra.domki.back();
    domek4.polozenie = { x + 100 ,y + 150 };
    domek4.typdomku = TypDomku::kFort;
    domek4.gracz = &g;
    domek4.poziom = 1;
    gra.ZmienLiczebnosc(domek4, 50);
    domek4.drogi.push_back(&domek);

    gra.domki.push_back(Domek());
    Domek& domek5 = gra.domki.back();
    domek5.polozenie = { x + 100 ,y + 250 };
    domek5.typdomku = TypDomku::kPole;
    domek5.gracz = &g;
    domek5.poziom = 0;
    gra.ZmienLiczebnosc(domek5, 50);
    domek5.drogi.push_back(&domek);
    domek5.drogi.push_back(&domek4);

    gra.domki.push_back(Domek());
    Domek& domek6 = gra.domki.back();
    domek6.polozenie = { x ,y + 250 };
    domek6.typdomku = TypDomku::kStajnia;
    domek6.gracz = &g;
    domek6.poziom = 4;
    gra.ZmienLiczebnosc(domek6, 50);
    domek6.drogi.push_back(&domek);
    domek6.drogi.push_back(&domek4);

    gra.domki.push_back(Domek());
    Domek& domek7 = gra.domki.back();
    domek7.polozenie = { x - 100 ,y + 250 };
    domek7.typdomku = TypDomku::kWieza;
    domek7.gracz = &g;
    domek7.poziom = 4;
    gra.ZmienLiczebnosc(domek7, 50);
    domek7.drogi.push_back(&domek);
    domek7.drogi.push_back(&domek4);

    gra.armie.push_back(Ludek(domek));
    Ludek & ludek = gra.armie.back();
    ludek.polozenie = { x + 100,y };
    ludek.gracz = &g;
    ludek.wyglad = Wyglad::kWojownik;
    ludek.tarcza = 0;
    gra.ZmienLiczebnosc(ludek, 240);

    gra.armie.push_back(Ludek(domek));
    Ludek & ludek2 = gra.armie.back();
    ludek2.polozenie = { x - 100,y };
    ludek2.gracz = &g;
    ludek2.tarcza = 90;
    ludek2.wyglad = Wyglad::kWojownik;
    gra.ZmienLiczebnosc(ludek2, 240);

    gra.armie.push_back(Ludek(domek));
    Ludek & ludek3 = gra.armie.back();
    ludek3.polozenie = { x + 100,y + 100 };
    ludek3.gracz = &g;
    ludek3.tarcza = 250;
    ludek3.wyglad = Wyglad::kWojownik;
    gra.ZmienLiczebnosc(ludek3, 240);

    gra.armie.push_back(Ludek(domek));
    Ludek & ludek4 = gra.armie.back();
    ludek4.polozenie = { x - 100,y + 100 };
    ludek4.gracz = &g;
    ludek4.tarcza = 250;
    ludek4.szybkosc_ludka = 3;
    ludek4.wyglad = Wyglad::kWojownik;
    gra.ZmienLiczebnosc(ludek4, 240);
}

void dodaj_konie(Rozgrywka & gra, Gracz& g, double x, double y)
{
    gra.armie.push_back(Ludek(gra.domki.front()));
    Ludek & ludek = gra.armie.back();
    ludek.polozenie = { x - 100,y - 100 };
    ludek.gracz = &g;
    ludek.wyglad = Wyglad::kWojownik;
    ludek.tarcza = 0;
    gra.ZmienLiczebnosc(ludek, 240);

    gra.armie.push_back(Ludek(gra.domki.front()));
    Ludek & ludek2 = gra.armie.back();
    ludek2.polozenie = { x ,y - 100 };
    ludek2.gracz = &g;
    ludek2.tarcza = 90;
    ludek2.wyglad = Wyglad::kWojownik;
    gra.ZmienLiczebnosc(ludek2, 240);

    gra.armie.push_back(Ludek(gra.domki.front()));
    Ludek & ludek3 = gra.armie.back();
    ludek3.polozenie = { x + 100,y - 100 };
    ludek3.gracz = &g;
    ludek3.tarcza = 250;
    ludek3.wyglad = Wyglad::kWojownik;
    gra.ZmienLiczebnosc(ludek3, 240);

    gra.armie.push_back(Ludek(gra.domki.front()));
    Ludek & ludek4 = gra.armie.back();
    ludek4.polozenie = { x - 100,y };
    ludek4.gracz = &g;
    ludek4.tarcza = 0;
    ludek4.szybkosc_ludka = 3;
    ludek4.wyglad = Wyglad::kWojownik;
    gra.ZmienLiczebnosc(ludek4, 240);

    gra.armie.push_back(Ludek(gra.domki.front()));
    Ludek & ludek5 = gra.armie.back();
    ludek5.polozenie = { x,y };
    ludek5.gracz = &g;
    ludek5.tarcza = 150;
    ludek5.szybkosc_ludka = 3;
    ludek5.wyglad = Wyglad::kWojownik;
    gra.ZmienLiczebnosc(ludek5, 240);

    gra.armie.push_back(Ludek(gra.domki.front()));
    Ludek & ludek6 = gra.armie.back();
    ludek6.polozenie = { x + 100,y };
    ludek6.gracz = &g;
    ludek6.tarcza = 250;
    ludek6.szybkosc_ludka = 3;
    ludek6.wyglad = Wyglad::kWojownik;
    gra.ZmienLiczebnosc(ludek6, 240);
}


Rozgrywka pokazowa_rozgrywka()
{
    Rozgrywka gra;
    //gracze
    gra.gracze.push_back(Gracz());
    Gracz& gracz0 = gra.gracze.back();
    gracz0.numer = 0; gracz0.nazwa = "NEUTRAL";
    gracz0.kolor = sf::Color(128, 128, 128);
    gracz0.aktywny = false;

    gra.gracze.push_back(Gracz());
    Gracz& gracz1 = gra.gracze.back();
    gracz1.numer = 1; gracz1.nazwa = "GRACZ1";
    gracz1.kolor = sf::Color::Red;

    gra.gracze.push_back(Gracz());
    Gracz& gracz2 = gra.gracze.back();
    gracz2.numer = 2; gracz2.nazwa = "KOMPUTER2";
    gracz2.kolor = sf::Color::Blue;

    gra.gracze.push_back(Gracz());
    Gracz& gracz3 = gra.gracze.back();
    gracz3.numer = 3; gracz3.nazwa = "KOMPUTER3";
    gracz3.kolor = sf::Color::Green;

    gra.gracze.push_back(Gracz());
    Gracz& gracz4 = gra.gracze.back();
    gracz4.numer = 4; gracz4.nazwa = "KOMPUTER4";
    gracz4.kolor = sf::Color::Yellow;

    dodaj_gracza(gra, gracz0, 500, 100);
    dodaj_gracza(gra, gracz1, 800, 100);
    dodaj_gracza(gra, gracz2, 800, 500);
    dodaj_gracza(gra, gracz3, 1100, 100);
    dodaj_gracza(gra, gracz4, 1100, 500);

    dodaj_konie(gra, gracz1, 500, 700);

    return gra;
}

MisjaUstawienia pokazowe_ustawienia()
{
    MisjaUstawienia res;
    res.do_ilu_wygranych = 3;
    return res;
}

std::shared_ptr<sfg::Window> probne_gui()
{
    sf::Texture sfgui_logo;
    sfgui_logo.loadFromFile("D:\\Fafa\\Domki\\Kod\\Grafika\\paper_small.png");
    sfgui_logo.setRepeated(true);

    sf::Sprite papierek;
    papierek.setTexture(sfgui_logo);
    papierek.setScale(8, 8);

    auto canvas = sfg::Canvas::Create();

    auto okno = sfg::Window::Create(sfg::Window::Style::BACKGROUND | sfg::Window::Style::SHADOW);
    okno->SetRequisition(sf::Vector2f(GUI::aplikacja().okno.getSize().x, 100));

    GUI::aplikacja().bottom_window(okno);

    auto lewy = sfg::Label::Create("Lewy panel");

    auto komunikat_koncowy = sfg::Label::Create("");
    komunikat_koncowy->SetId("Naglowek");
    komunikat_koncowy->SetAlignment(sf::Vector2f(0.5, 0.5));
    komunikat_koncowy->SetText("GRATULACJE DLA GRACZA");
    GUI::aplikacja().pulpit.SetProperty("Label", "Color", sf::Color::White);
    GUI::aplikacja().pulpit.SetProperty("Label", "FontSize", 28);
    GUI::aplikacja().pulpit.SetProperty("Label#Naglowek", "FontSize", 48);

    auto prawy = sfg::Label::Create("Prawy panel");

    auto table = sfg::Table::Create();
    GUI::aplikacja().pulpit.SetProperty("Label", "BorderColor", sf::Color(79, 45, 4));
    GUI::aplikacja().pulpit.SetProperty("Label", "BorderWidth", 8);
    table->Attach(lewy, sf::Rect<sf::Uint32>(0, 0, 1, 1));
    table->Attach(canvas, sf::Rect<sf::Uint32>(1, 0, 1, 1));
    table->Attach(komunikat_koncowy, sf::Rect<sf::Uint32>(1, 0, 1, 1));
    table->Attach(prawy, sf::Rect<sf::Uint32>(2, 0, 1, 1));

    okno->Add(table);

    canvas->Bind();
    canvas->Clear();
    canvas->Draw(papierek);

    GUI::aplikacja().set_active_window(okno);
    return okno;
}

int pokazowa_misja()
{
    sf::RenderWindow& window = GUI::aplikacja().okno;
    GUI::aplikacja().setup_theme();

    // tworzymy rozgrywke
    Rozgrywka rozgrywka = pokazowa_rozgrywka();
    MisjaUstawienia ustawienia = pokazowe_ustawienia();
    // przygotowujemy dzialaczy
    Wyswietlacz wyswietlacz(rozgrywka);
    wyswietlacz.Zaladuj("rycerze_hd");
    MyszDecydent myszkaGracza(window, rozgrywka, rozgrywka.Graczu(1));

    // przygotuj punkty kontrolne
    myszkaGracza.punkty_kontrolne[rozgrywka.WskaznikDomek(23)] = rozgrywka.WskaznikDomek(23);
    myszkaGracza.punkty_kontrolne[rozgrywka.WskaznikDomek(24)] = rozgrywka.WskaznikDomek(22);

    OznaczaczWyborow ruchGracza(myszkaGracza);

    auto gujak = interfejs_rozgrywki(nullptr, ustawienia, rozgrywka, wyswietlacz, nullptr);
    sf::View view = wysrodkowany_widok(rozgrywka.domki, gujak->GetAllocation().height);
    window.setView(view);

    Ruszacz ruszacz;
    ruszacz.rozgrywka = &rozgrywka;
    ruszacz.szybkosc *= 0.0001;

    //czasomierz
    auto czasomierz = clock();
    int czasik = 0;
    //APM
    long long akcje = 0;
    double czas_przeminal = clock();
    while (window.isOpen())
    {
        sf::Event event;

        myszkaGracza.Przetworz(); // puste
        while (window.pollEvent(event))
        {
            myszkaGracza.Przetworz(event);
            if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::KeyReleased)akcje++;
            switch (event.type)
            {
            case sf::Event::KeyReleased:
                switch (event.key.code)
                {
                case sf::Keyboard::Escape:
                    return 0;
                case sf::Keyboard::F3:
                    wyswietlacz.ZaladujInne();
                    break;
                case sf::Keyboard::F4:
                    //zakonczenie_meczu()
                    //wyswietlacz.ZaladujInne();
                    break;
                case sf::Keyboard::Q:
                    rozgrywka.PoddajGracza(rozgrywka.Graczu("KOMPUTER2"));
                    break;
                }
                break;
            }
            if (event.type == sf::Event::Closed)
                window.close();
        }

        ruszacz.Ruszaj(0.0001);

        window.clear();

        interfejs_rozgrywki(gujak, ustawienia, rozgrywka, wyswietlacz, ruchGracza.WybranyDomek());

        GUI::aplikacja().show_bottom_gui(view, gujak);

        wyswietlacz.WyswietlTlo(window);
        ruchGracza.Wyswietlaj(window);
        wyswietlacz.Wyswietlaj(window);

        GUI::aplikacja().okno.display();

        Sleep(16);
    }
    return 0;
}

int main(int argc, const char * argv[]) {

    pokazowa_misja();

    return 0;
}
