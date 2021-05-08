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

class DecydentEdytor {
public:
    DecydentEdytor(sf::RenderWindow& okno, Rozgrywka& rozgrywka);
    
    Twor* wybrany;
    Domek tworzony;
    sf::Vector2f miejsce_tworzenia;


    bool tworz;
    std::pair<bool, bool> zapisuj;

    int  ladowanie_ludkow;
    

    void Przetworz(sf::Event zdarzenie);
    void Wykonaj();

private:
    sf::RenderWindow & okno;
    Rozgrywka& rozgrywka;
};

DecydentEdytor::DecydentEdytor(sf::RenderWindow& okno, Rozgrywka& rozgrywka) : okno(okno), rozgrywka(rozgrywka)
{
    wybrany = NULL;
    tworzony.liczebnosc = -1;
    tworz = false;
    ladowanie_ludkow = 0;
}

sf::Vector2f ustaw_pozycje(sf::Vector2f polozenie_kliku, float szerokosc_podzialu, float wysokosc_podzialu) {
    sf::Vector2f koryguj;
    koryguj.x = (float)((int)polozenie_kliku.x / (int)szerokosc_podzialu) * szerokosc_podzialu + szerokosc_podzialu / 2.0;
    koryguj.y = (float)((int)polozenie_kliku.y / (int)wysokosc_podzialu) * wysokosc_podzialu + wysokosc_podzialu / 2.0;
    return koryguj;
}

void DecydentEdytor::Przetworz(sf::Event zdarzenie) {
    if (zdarzenie.type == sf::Event::MouseButtonPressed) {

        sf::Vector2i pixelPos = sf::Mouse::getPosition(okno);
        miejsce_tworzenia = ustaw_pozycje(okno.mapPixelToCoords(pixelPos), 80.0, 90.0);
        Twor* klikniety = rozgrywka.Zlokalizuj(miejsce_tworzenia.x, miejsce_tworzenia.y);

        if (zdarzenie.mouseButton.button == sf::Mouse::Left) {
            wybrany = klikniety;
        }

        else if (tworzony.liczebnosc != -1 && klikniety == NULL && zdarzenie.mouseButton.button == sf::Mouse::Right) {
            tworz = true;
        }
    }

    else if (zdarzenie.type == sf::Event::KeyPressed) {
        
        switch (zdarzenie.key.code) {
        case sf::Keyboard::Backspace: {
            if (wybrany != NULL) {
                if(IsType<Domek>(wybrany))rozgrywka.ZniszczDomek((Domek*)wybrany);
                wybrany = NULL;
            }
            break;
        }
            case sf::Keyboard::Num0: {

                tworzony.liczebnosc = -1;
                rozgrywka.pozostale.remove(&tworzony);
                break;
            }
            case sf::Keyboard::Tilde: {

                if (tworzony.liczebnosc == -1) {
                    tworzony = rozgrywka.stworz_domyslny_domek();
                    tworzony.typdomku = TypDomku::kPole;
                    rozgrywka.ZmienPoziom(tworzony, 0);
                    rozgrywka.pozostale.push_back(&tworzony);
                }
                tworzony.typdomku = TypDomku::kPole;
                rozgrywka.ZmienPoziom(tworzony, 0);

                break;
            }
            case sf::Keyboard::Num1: {
                
                if (tworzony.liczebnosc == -1) {
                    tworzony = rozgrywka.stworz_domyslny_domek();
                    tworzony.typdomku = TypDomku::kMiasto;
                    rozgrywka.pozostale.push_back(&tworzony);
                    }   
                if (tworzony.poziom)tworzony.typdomku = TypDomku::kMiasto;

                break;
            }
            case sf::Keyboard::Num2: {

                if (tworzony.liczebnosc == -1) {
                    tworzony = rozgrywka.stworz_domyslny_domek();
                    tworzony.typdomku = TypDomku::kFort;
                    rozgrywka.pozostale.push_back(&tworzony);
                }
                if (tworzony.poziom)tworzony.typdomku = TypDomku::kFort;

                break;
            }
            case sf::Keyboard::Num3: {

                if (tworzony.liczebnosc == -1) {
                    tworzony = rozgrywka.stworz_domyslny_domek();
                    tworzony.typdomku = TypDomku::kZbrojownia;
                    rozgrywka.pozostale.push_back(&tworzony);
                }
                if (tworzony.poziom)tworzony.typdomku = TypDomku::kZbrojownia;

                break;
            }
            case sf::Keyboard::Num4: {

                if (tworzony.liczebnosc == -1) {
                    tworzony = rozgrywka.stworz_domyslny_domek();
                    tworzony.typdomku = TypDomku::kWieza;
                    rozgrywka.pozostale.push_back(&tworzony);
                }
                if (tworzony.poziom)tworzony.typdomku = TypDomku::kWieza;

                break;
            }
            case sf::Keyboard::Num5: {
                
                if (tworzony.liczebnosc == -1) {
                    tworzony = rozgrywka.stworz_domyslny_domek();
                    tworzony.typdomku = TypDomku::kStajnia;
                    rozgrywka.pozostale.push_back(&tworzony);
                }
                if(tworzony.poziom)tworzony.typdomku = TypDomku::kStajnia;

                break;
            }
           case sf::Keyboard::Numpad0: {

               if (tworzony.liczebnosc == -1) {
                   tworzony = rozgrywka.stworz_domyslny_domek();
                   rozgrywka.pozostale.push_back(&tworzony);
               }
               tworzony.gracz = &rozgrywka.Graczu(0);

                break;
            }
           case sf::Keyboard::Numpad1: {

               if (tworzony.liczebnosc == -1) {
                   tworzony = rozgrywka.stworz_domyslny_domek();
                   rozgrywka.pozostale.push_back(&tworzony);
               }
               if (rozgrywka.gracze.size() > 1)tworzony.gracz = &rozgrywka.Graczu(1);
               else tworzony.gracz = &rozgrywka.Graczu(0);

               break;
           }
           case sf::Keyboard::Numpad2: {

               if (tworzony.liczebnosc == -1) {
                   tworzony = rozgrywka.stworz_domyslny_domek();
                   rozgrywka.pozostale.push_back(&tworzony);
               }
               if (rozgrywka.gracze.size() > 2)tworzony.gracz = &rozgrywka.Graczu(2);
               else tworzony.gracz = &rozgrywka.Graczu(0);

               break;
           }
           case sf::Keyboard::Numpad3: {

               if (tworzony.liczebnosc == -1) {
                   tworzony = rozgrywka.stworz_domyslny_domek();
                   rozgrywka.pozostale.push_back(&tworzony);
               }
               if (rozgrywka.gracze.size() > 3)tworzony.gracz = &rozgrywka.Graczu(3);
               else tworzony.gracz = &rozgrywka.Graczu(0);

               break;
           }
           case sf::Keyboard::Numpad4: {

               if (tworzony.liczebnosc == -1) {
                   tworzony = rozgrywka.stworz_domyslny_domek();
                   rozgrywka.pozostale.push_back(&tworzony);
               }
               if (rozgrywka.gracze.size() > 4)tworzony.gracz = &rozgrywka.Graczu(4);
               else tworzony.gracz = &rozgrywka.Graczu(0);
               break;
           }
           case sf::Keyboard::Numpad5: {

               if (tworzony.liczebnosc == -1) {
                   tworzony = rozgrywka.stworz_domyslny_domek();
                   rozgrywka.pozostale.push_back(&tworzony);
               }
               if (rozgrywka.gracze.size() > 5)tworzony.gracz = &rozgrywka.Graczu(5);
               else tworzony.gracz = &rozgrywka.Graczu(0);

               break;
           }
           case sf::Keyboard::Numpad6: {

               if (tworzony.liczebnosc == -1) {
                   tworzony = rozgrywka.stworz_domyslny_domek();
                   rozgrywka.pozostale.push_back(&tworzony);
               }
               if (rozgrywka.gracze.size() > 6)tworzony.gracz = &rozgrywka.Graczu(6);
               else tworzony.gracz = &rozgrywka.Graczu(0);

               break;
           }
           case sf::Keyboard::Numpad7: {

               if (tworzony.liczebnosc == -1) {
                   tworzony = rozgrywka.stworz_domyslny_domek();
                   rozgrywka.pozostale.push_back(&tworzony);
               }
               if (rozgrywka.gracze.size() > 7)tworzony.gracz = &rozgrywka.Graczu(7);
               else tworzony.gracz = &rozgrywka.Graczu(0);

               break;
           }
           case sf::Keyboard::Numpad8: {

               if (tworzony.liczebnosc == -1) {
                   tworzony = rozgrywka.stworz_domyslny_domek();
                   rozgrywka.pozostale.push_back(&tworzony);
               }
               if (rozgrywka.gracze.size() > 8)tworzony.gracz = &rozgrywka.Graczu(8);
               else tworzony.gracz = &rozgrywka.Graczu(0);

               break;
           }
           case sf::Keyboard::Numpad9: {

               if (tworzony.liczebnosc == -1) {
                   tworzony = rozgrywka.stworz_domyslny_domek();
                   rozgrywka.pozostale.push_back(&tworzony);
               }
               if (rozgrywka.gracze.size() > 9)tworzony.gracz = &rozgrywka.Graczu(9);
               else tworzony.gracz = &rozgrywka.Graczu(0);

               break;
           }
            case sf::Keyboard::Add: {

                if (tworzony.liczebnosc == -1) {
                    tworzony = rozgrywka.stworz_domyslny_domek();
                    rozgrywka.pozostale.push_back(&tworzony);
                }
                if (tworzony.poziom < 5) {
                    rozgrywka.ZmienPoziom(tworzony, tworzony.poziom + 1);
                }

                break;
            }
            case sf::Keyboard::Subtract: {

                if (tworzony.liczebnosc == -1) {
                    tworzony = rozgrywka.stworz_domyslny_domek();
                    rozgrywka.pozostale.push_back(&tworzony);
                }
                if (tworzony.poziom > 0) {
                    rozgrywka.ZmienPoziom(tworzony, tworzony.poziom - 1);
                }

                break;
            }
            case sf::Keyboard::Tab: {
                if(!ladowanie_ludkow)ladowanie_ludkow = clock();
                break;
            }
            case sf::Keyboard::S: {
                zapisuj.first = true;
                break;
            }
            case sf::Keyboard::LControl: {
                zapisuj.second = true;
                break;
            }
            case sf::Keyboard::RControl: {
                zapisuj.second = true;
                break;
            }
        }
    }

    else if (zdarzenie.type == sf::Event::KeyReleased && zdarzenie.key.code == sf::Keyboard::Tab) {

        if (tworzony.liczebnosc == -1) {
            tworzony = rozgrywka.stworz_domyslny_domek();
            rozgrywka.pozostale.push_back(&tworzony);
        }
        if (ladowanie_ludkow) ladowanie_ludkow = 0;
        else if (tworzony.typdomku == TypDomku::kPole) tworzony.liczebnosc = 0;
        else {
            if (tworzony.liczebnosc < tworzony.max_liczebnosc / 2.0) tworzony.liczebnosc = tworzony.max_liczebnosc / 2.0;
            else if (tworzony.liczebnosc < tworzony.max_liczebnosc) tworzony.liczebnosc = tworzony.max_liczebnosc;
            else tworzony.liczebnosc = 0.0;
        }
    }

    else if (zdarzenie.type == sf::Event::KeyReleased && zapisuj.first && zapisuj.second && (zdarzenie.key.code == sf::Keyboard::S || zdarzenie.key.code == sf::Keyboard::LControl || zdarzenie.key.code == sf::Keyboard::RControl )) {
          znajdz_miejsce_pod_zapis(rozgrywka);
          zapisuj.first = false;
          zapisuj.second = false;
        }
    else if (zdarzenie.type == sf::Event::KeyReleased && zdarzenie.key.code == sf::Keyboard::S) {
        zapisuj.first = false;
    }
    else if (zdarzenie.type == sf::Event::KeyReleased && (zdarzenie.key.code == sf::Keyboard::LControl || zdarzenie.key.code == sf::Keyboard::RControl)) {
        zapisuj.second = false;
    }
}

void DecydentEdytor::Wykonaj() {
    if (tworzony.liczebnosc != -1 && tworz) {

        tworzony.polozenie.x = miejsce_tworzenia.x;
        tworzony.polozenie.y = miejsce_tworzenia.y;
        rozgrywka.domki.push_back(tworzony);

        tworz = false;
    }
    else if (tworzony.liczebnosc != -1 && ladowanie_ludkow && (double)(clock() - ladowanie_ludkow) / CLOCKS_PER_SEC > 0.5) tworzony.liczebnosc += ((double)(clock() - ladowanie_ludkow) / CLOCKS_PER_SEC - 0.5) * tworzony.max_liczebnosc;
}

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

    auto gujak = interfejs_rozgrywki(nullptr, ustawienia, rozgrywka, wyswietlacz, nullptr, nullptr);
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

        interfejs_rozgrywki(gujak, ustawienia, rozgrywka, wyswietlacz, (decedek.tworzony.liczebnosc > -1 ? &decedek.tworzony : NULL) , decedek.wybrany);

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
