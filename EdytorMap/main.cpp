#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include <SFML/Graphics.hpp>

#include "easylogging++.h"
INITIALIZE_EASYLOGGINGPP

#include "../Domki/gui.h"
#include "../Domki/misja.h"
#include "../Domki/os.h"
#include "decydentEdytora.h"


using namespace std;

#include <iterator>

void dodaj_domek(Rozgrywka &gra, Gracz &g, double x, double y, TypDomku typ, int poziom, int liczebnosc = 0) {
    gra.domki.push_back(Domek());
    Domek& domek = gra.domki.back();
    domek.polozenie = { x,y };
    domek.typdomku = typ;
    domek.gracz = &g;
    domek.poziom = poziom;
    domek.max_liczebnosc = 100 << (poziom - 1);
    gra.ZmienLiczebnosc(domek, liczebnosc);
}

Rozgrywka pokazowa_rozgrywka()
{
    Rozgrywka gra;
    //gracze

    sf::Color kolory[] = { sf::Color(255,255,255) , sf::Color(255,0,0), sf::Color(0,0,255), sf::Color(0,255,0),      // 0 - bialy, 1 - czerwony, 2 - niebieski, 3 - zielony
    sf::Color(255,255,0) , sf::Color(255,128,0), sf::Color(0,255,255), sf::Color(127,0,255),                              // 4 - zolty, 5 - pomaranczowy, 6 - jasnoniebieski, 7- fioletowy
    sf::Color(102,102,0), sf::Color(51,25,0)};                                                                                                                                        // 8 - ciemnozloty 9 - brazowy

    string nazwy[] = { "BIALY", "CZERWONY", "NIEBIESKI", "ZIELONY", "ZOLTY", "POMARANCZOWY", "JASNONIEBIESKI", "FIOLETOWY", "CIEMNOZLOTY", "BRAZOWY" };

    for (int nr = 0; nr <= 9; nr++) {
        gra.gracze.push_back(Gracz());
        Gracz& gracz = gra.gracze.back();
        gracz.numer = nr;
        gracz.nazwa = nazwy[nr];
        gracz.kolor = kolory[nr];
        if (!nr) gracz.aktywny = false;
    }

    return gra;
}

MisjaUstawienia pokazowe_ustawienia()
{
    MisjaUstawienia res;
    res.do_ilu_wygranych = 3;
    return res;
}

int buduj_mape()
{
    sf::RenderWindow& window = GUI::aplikacja().okno;
    GUI::aplikacja().setup_theme();

    // tworzymy rozgrywke
    Rozgrywka rozgrywka = pokazowa_rozgrywka();
    MisjaUstawienia ustawienia = pokazowe_ustawienia();
    // przygotowujemy dzialaczy
    Wyswietlacz wyswietlacz(rozgrywka);
    wyswietlacz.Zaladuj("rycerze_hd");

    MyszDecydent myszkaGracza(window, rozgrywka, rozgrywka.Graczu(1));//ZMIANA KLIKACZ

    DecydentEdytor decedek(window, rozgrywka);

    std::shared_ptr<sfg::Window> gujak = pokazowy_interfejs(nullptr, ustawienia, rozgrywka, wyswietlacz, nullptr, nullptr);
    sf::View view = sf::View(sf::FloatRect(0, 0, 1600, 900));
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

        //myszkaGracza.Przetworz(); // puste
        while (window.pollEvent(event))
        {
            decedek.Przetworz(event);
            decedek.Wykonaj();
            switch (event.type)
            {
            case sf::Event::KeyReleased:
                switch (event.key.code)
                {
                case sf::Keyboard::Escape:
                    window.close();
                    return 0;
                    break;
                case sf::Keyboard::F3:
                    wyswietlacz.ZaladujInne();
                    break;
                case sf::Keyboard::F4:
                    //zakonczenie_meczu()
                    //wyswietlacz.ZaladujInne();
                    break;
                }
            }
            if (event.type == sf::Event::Closed)
                window.close();
        }

        ruszacz.Ruszaj(0.0001);

        window.clear();

        pokazowy_interfejs(gujak, ustawienia, rozgrywka, wyswietlacz, (decedek.tworzony.liczebnosc > -1 ? &decedek.tworzony : NULL) , decedek.wybrany);

        GUI::aplikacja().show_bottom_gui(view, gujak);

        wyswietlacz.WyswietlTlo(window);
        wyswietlacz.Wyswietlaj(window);

        GUI::aplikacja().okno.display();

        Sleep(16);
    }
    return 0;
}

int main(int argc, const char * argv[]) {

    buduj_mape();

    return 0;
}
