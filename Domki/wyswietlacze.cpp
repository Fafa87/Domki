#include "Narzedzia\AnimatedSprite.hpp"
#include "wyswietlacze.h"

#include "Thor/Shapes/Arrow.hpp"

#include<set>
#include<string>
#include<ctime>
#include <filesystem>

#define _USE_MATH_DEFINES
#include<math.h>

map<string, sf::Texture*> ZestawAnimacji::ZaladowaneObrazy;

sf::Texture * ZestawAnimacji::ZaladujIZapamietaj(string & sciezka)
{
    sf::Texture* tekstura;
    if (ZestawAnimacji::ZaladowaneObrazy.count(sciezka))
        tekstura = ZestawAnimacji::ZaladowaneObrazy[sciezka];
    else
    {
        tekstura = new sf::Texture();
        tekstura->loadFromFile(sciezka);
        tekstura->setSmooth(true);
        tekstura->generateMipmap();
        ZestawAnimacji::ZaladowaneObrazy[sciezka] = tekstura;
    }
    return tekstura;
}

Animation ZestawAnimacji::ZaladujAnimacje(string& sciezka)
{
    Animation res;
    sf::Texture* tekstura = ZestawAnimacji::ZaladujIZapamietaj(sciezka);
    res.setSpriteSheet(*tekstura);

    int dlugosc_klatki = 400;
    if ((int)tekstura->getSize().x == (int)tekstura->getSize().y)
    {
        dlugosc_klatki = (int)tekstura->getSize().x;
    }
    else 
    {
        if (tekstura->getSize().x % tekstura->getSize().y == 0)
            dlugosc_klatki = tekstura->getSize().y;
        else {
            if (abs((int)tekstura->getSize().x % dlugosc_klatki) > 100)
                dlugosc_klatki = 600;
            if (abs((int)tekstura->getSize().x % dlugosc_klatki) > 100)
                dlugosc_klatki = 800;
        }
    }

    int klatek = (tekstura->getSize().x + dlugosc_klatki / 2) / dlugosc_klatki;
    for (int i = 0; i < klatek; i++)
        res.addFrame(sf::IntRect(dlugosc_klatki * i, 0, dlugosc_klatki, tekstura->getSize().y));

    if (klatek <= 4)
    {
        // wczytaj powrotne
        for (int i = klatek - 1; i >= 1; i--)
            res.addFrame(sf::IntRect(dlugosc_klatki * i, 0, dlugosc_klatki, tekstura->getSize().y));
    }
    return res;
}

Animation ZestawAnimacji::PobierzAnimacjePoziomu(int poziom)
{
    poziom = min(poziom, (int)wszystkieAnimacje.size() - 1);
    return wszystkieAnimacje[poziom];
}

sf::IntRect ZestawAnimacji::Rozmiar()
{
    return this->PobierzAnimacjePoziomu(1).getFrame(0);
}

ZestawAnimacji ZestawAnimacji::ZaladujZPliku(string & sciezka_szablon)
{
    ZestawAnimacji res;
    auto szablon = sf::String(sciezka_szablon);

    auto bez_poziomu = szablon;
    bez_poziomu.replace("{}", "");

    if (std::experimental::filesystem::exists(szablon.toAnsiString()))
        res.wszystkieAnimacje.push_back(ZaladujAnimacje(szablon.toAnsiString()));
    else if (std::experimental::filesystem::exists(bez_poziomu.toAnsiString()))
        res.wszystkieAnimacje.push_back(ZaladujAnimacje(bez_poziomu.toAnsiString()));
    else
    {
        auto z_poziomem = szablon;
        z_poziomem.replace("{}", "_" + to_string(0));

        if (std::experimental::filesystem::exists(z_poziomem.toAnsiString()))
            res.wszystkieAnimacje.push_back(ZaladujAnimacje(z_poziomem.toAnsiString()));
        else
            res.wszystkieAnimacje.push_back(Animation());

        for (int i = 1; i < 10; i++)
        {
            auto z_poziomem = szablon;
            z_poziomem.replace("{}", "_" + to_string(i));

            if (std::experimental::filesystem::exists(z_poziomem.toAnsiString()))
                res.wszystkieAnimacje.push_back(ZaladujAnimacje(z_poziomem.toAnsiString()));
            else
                break;
        }
    }
    return res;
}


Wyswietlacz::Wyswietlacz(Rozgrywka & rozgrywka) : rozgrywka(rozgrywka)
{
    czcionka.loadFromFile("Grafika\\waltographUI.ttf");

    widok = nullptr;
}


void Wyswietlacz::Zaladuj(string wybrana_skora)
{
    skorka = wybrana_skora;
    obrazek_tworow[Wyglad::kWojownik] = ZestawAnimacji::ZaladujZPliku("Grafika\\" + skorka + "\\wojownik{}.png");

    obrazek_tworow[Wyglad::kMiasto] = ZestawAnimacji::ZaladujZPliku("Grafika\\" + skorka + "\\miasto{}.png");
    
    obrazek_tworow[Wyglad::kFort] = ZestawAnimacji::ZaladujZPliku("Grafika\\" + skorka + "\\fort{}.png");
    obrazek_tworow[Wyglad::kWieza] = ZestawAnimacji::ZaladujZPliku("Grafika\\" + skorka + "\\wieza{}.png");

    obrazek_tworow[Wyglad::kZbrojownia] = ZestawAnimacji::ZaladujZPliku("Grafika\\" + skorka + "\\zbrojownia{}.png");
    obrazek_tworow[Wyglad::kStajnia] = ZestawAnimacji::ZaladujZPliku("Grafika\\" + skorka + "\\stajnia{}.png");

    obrazek_tworow[Wyglad::kPole] = ZestawAnimacji::ZaladujZPliku("Grafika\\" + skorka + "\\pole.png");

    obrazek_tla = *ZestawAnimacji::ZaladujIZapamietaj("Grafika\\" + skorka + "\\bruk.png");
    obrazek_tla.setRepeated(true);
}

void Wyswietlacz::ZaladujInne()
{
    this->Zaladuj(next_folder_within_folder("Grafika\\" + skorka));
}

sf::RectangleShape Wyswietlacz::PobierzWyglad(Twor * twor)
{
    if (wyglad_tworow.count(twor))
    {
        return wyglad_tworow[twor];
    }
    return sf::RectangleShape();
}

void Wyswietlacz::WyswietlTlo(sf::RenderWindow& okno, sf::View& tlo, sf::View* nowy_widok = nullptr)
{
    sf::Sprite wyglad_tlo(obrazek_tla);
    if(nowy_widok != nullptr)
        widok = nowy_widok;
    sf::Vector2f widok_center = tlo.getCenter(), widok_size = tlo.getSize();
    
    wyglad_tlo.setTextureRect({ (int)(widok_center.x - widok_size.x / 2.0), (int)(widok_center.y - widok_size.y / 2.0), (int)(widok_size.x), (int)(widok_size.y) });
    okno.draw(wyglad_tlo);

    // namaluj drogi
    for (auto dom : rozgrywka.domki)
    {
        for (auto dokad : dom.drogi) if(dokad->uid < dom.uid) // maluj tylko w jedną stronę
        {
            sf::Vertex linia[] =
            {
                sf::Vertex(sf::Vector2f(dom.polozenie.x, dom.polozenie.y + 15), sf::Color(70, 40, 0)),
                sf::Vertex(sf::Vector2f(dokad->polozenie.x, dokad->polozenie.y + 15), sf::Color(70, 40, 0))
            };
            int odleglosc = sqrt(pow(dokad->polozenie.x - dom.polozenie.x, 2) + pow(dokad->polozenie.y - dom.polozenie.y, 2));
            sf::RectangleShape linijka(sf::Vector2f(odleglosc, 3));
            linijka.setPosition(linia[0].position);

            linijka.setRotation(atan2(linia[1].position.y - linia[0].position.y, linia[1].position.x - linia[0].position.x) / M_PI * 180);
            linijka.setFillColor(sf::Color(150, 75, 0));
            linijka.setOutlineColor(sf::Color(70, 40, 0));
            linijka.setOutlineThickness(1);

            okno.draw(linijka);
        }
    }
}

inline PD translacja_pozycji(PD polozenie, PD korekta, PD min, PD max, PD wielkosc_docelowa) {
    PD rozrzut = max - min;
    wielkosc_docelowa -= korekta * 2.0;
    PD wynikowe = { (rozrzut.x > 0 ? wielkosc_docelowa.x * (polozenie.x - min.x) / rozrzut.x : 0) , (rozrzut.y > 0 ? wielkosc_docelowa.y * (polozenie.y - min.y) / rozrzut.y : 0) };
    wynikowe += korekta;
    return wynikowe;
}

sf::Image Wyswietlacz::StworzMinimape(PD wielkosc) {
    sf::Image minimapa;
    minimapa.create(wielkosc.x, wielkosc.y, sf::Color(140, 255, 140));

    rozgrywka.UstalRozrzutDomkow();
    PD  min = rozgrywka.min_rog, max = rozgrywka.max_rog;
    PD korekta = { wielkosc.x / 10.0 , wielkosc.y / 10.0 };
    if (min.x == max.x) korekta.x = wielkosc.x / 2;
    if (min.y == max.y) korekta.y = wielkosc.y / 2;

    for (auto dom : rozgrywka.domki)
    {
        PD dom_poz = translacja_pozycji(dom.polozenie, korekta, min, max, wielkosc);
        for (auto dokad : dom.drogi) if (dokad->uid < dom.uid)
        {
            PD dokad_poz = translacja_pozycji(dokad->polozenie, korekta, min, max, wielkosc);
            for (double i = dom_poz.x, j = dom_poz.y, i_delta = dokad_poz.x - dom_poz.x, j_delta = dokad_poz.y - dom_poz.y, dist = sqrt(i_delta * i_delta + j_delta * j_delta); (abs(dokad_poz.x - i) > 3.0 || abs(dokad_poz.y - j) > 3.0);) {
                minimapa.setPixel((int)i, (int)j, sf::Color(150, 75, 0));
                i += i_delta / dist; j += j_delta / dist;
            }

        }
    }
    for (auto dom : rozgrywka.domki) {
        PD dom_poz = translacja_pozycji(dom.polozenie, korekta, min, max, wielkosc);
        for(int i = -3; i <= 3; i++)
           for(int j = -3; j <= 3; j++)
               minimapa.setPixel(dom_poz.x + i, dom_poz.y + j, dom.gracz->kolor);
    }
    for (auto ludek : rozgrywka.armie) {
        PD ludek_poz = translacja_pozycji(ludek.polozenie, korekta, min, max, wielkosc);
        float stosunek_ludka_do_domku = ludek.liczebnosc / dynamic_cast<Domek&>(*ludek.skad).liczebnosc;
        int sila_ludka = (stosunek_ludka_do_domku >= 0.5 ? 2 : 1);
        for (int i = -sila_ludka; i <= sila_ludka; i++)
            for (int j = -sila_ludka; j <= sila_ludka; j++)
                minimapa.setPixel(ludek_poz.x + i, ludek_poz.y + j, ludek.gracz->kolor);
    }

    if (widok != nullptr) {
        PD centrum = translacja_pozycji({ widok->getCenter().x, widok->getCenter().y }, korekta, min, max, wielkosc), rozmiar = translacja_pozycji({ widok->getSize().x, widok->getSize().y }, korekta, min, max, wielkosc);
        PD min_rog = centrum - rozmiar / 2.0, max_rog = centrum + rozmiar / 2.0;
        for (int i = min_rog.x, j = min_rog.y; i <= max_rog.x; i++) minimapa.setPixel(std::max(std::min((int)wielkosc.x - 1, i), 0), std::max(std::min((int)wielkosc.y - 1, j), 0),  sf::Color(213, 181, 110));
        for (int i = max_rog.x, j = min_rog.y; j <= max_rog.y; j++) minimapa.setPixel(std::max(std::min((int)wielkosc.x - 1, i), 0), std::max(std::min((int)wielkosc.y - 1, j), 0), sf::Color(213, 181, 110));
        for (int i = max_rog.x, j = max_rog.y; i >= min_rog.x; i--) minimapa.setPixel(std::max(std::min((int)wielkosc.x - 1, i), 0), std::max(std::min((int)wielkosc.y - 1, j), 0), sf::Color(213, 181, 110));
        for (int i = min_rog.x, j = max_rog.y; j >= min_rog.y; j--) minimapa.setPixel(std::max(std::min((int)wielkosc.x - 1, i), 0), std::max(std::min((int)wielkosc.y - 1, j), 0), sf::Color(213, 181, 110));
    }

    return minimapa;
}

sf::View Wyswietlacz::ProstyWidokMapy(int wysokosc) {
    for (Domek& domek : rozgrywka.domki)
        domek.polozenie = translacja_pozycji(domek.polozenie, { 160.0, (900.0 - (double)wysokosc) / 10.0 }, rozgrywka.min_rog, rozgrywka.max_rog, { 1600.0, (900.0 - (double)wysokosc)});
    return sf::View({ 0, 0, 1600, 900 });
}

void Wyswietlacz::UaktualnijWyglad(Twor* twor) {

    if (IsType<Ludek>(twor)) {
        Ludek& ludek = *((Ludek*)twor);

        double procent_tarczy = ludek.tarcza / (double)ludek.liczebnosc;
        ludek.wyglad_rodzaj = procent_tarczy > 1.0 ? 3 : (procent_tarczy > 0.25 ? 2 : 1);
        ludek.wyglad_rodzaj += ludek.szybkosc_ludka > 2 ? 3 : 0;
    }

    else if (IsType<Domek>(twor)) {
        Domek& dom = *((Domek*)twor);

        dom.wyglad_rodzaj = dom.poziom;

        if (dom.typdomku == TypDomku::kMiasto)
            dom.wyglad = Wyglad::kMiasto;
        else if (dom.typdomku == TypDomku::kFort)
            dom.wyglad = Wyglad::kFort;
        else if (dom.typdomku == TypDomku::kZbrojownia)
            dom.wyglad = Wyglad::kZbrojownia;
        else if (dom.typdomku == TypDomku::kWieza)
            dom.wyglad = Wyglad::kWieza;
        else if (dom.typdomku == TypDomku::kStajnia)
            dom.wyglad = Wyglad::kStajnia;
        else if (dom.typdomku == TypDomku::kPole)
            dom.wyglad = Wyglad::kPole;
        else dom.wyglad = Wyglad::kNieznany;
    }
}

void Wyswietlacz::Wyswietlaj(sf::RenderWindow & okno)
{
    set<Twor*> wszystkie_obiekty;
    for (auto& dom : rozgrywka.domki)
        wszystkie_obiekty.insert(&dom);
    for (auto& lud : rozgrywka.armie)
        wszystkie_obiekty.insert(&lud);
    for (auto& pozos : rozgrywka.pozostale)
        wszystkie_obiekty.insert(pozos);

    // usuń wyglądy których już nie ma
    vector<Twor*> do_usuniecia;
    for (auto& wyg_map : wyglad_tworow)
        if (!wszystkie_obiekty.count(wyg_map.first))
            do_usuniecia.push_back(wyg_map.first);

    for (auto twor : do_usuniecia)
        wyglad_tworow.erase(twor);


    // uaktualnijmy ich stan
    for (auto& twor : wszystkie_obiekty)
    {
        UaktualnijWyglad(twor);

        bool rysuj = (std::find(rozgrywka.pozostale.begin(), rozgrywka.pozostale.end(), twor) == rozgrywka.pozostale.end());

        auto& wyglad = wyglad_tworow[twor];
        auto zestaw_animacja_tworu = obrazek_tworow[twor->wyglad];
        auto animacja_tworu = zestaw_animacja_tworu.PobierzAnimacjePoziomu(twor->wyglad_rodzaj);
        wyglad.setPosition(twor->polozenie.x, twor->polozenie.y);

        int liczba_ramek = animacja_tworu.getSize();
        int dlugosc_jednego = 10;
        int wysokosc_jednego = 10;
        if (liczba_ramek > 0)
        {
            auto rozmiar = animacja_tworu.getFrame(0);
            dlugosc_jednego = rozmiar.width;
            wysokosc_jednego = rozmiar.height;
        }

        // tymczasowo trochę większe nowego wieśniaka
        double rozmiar = twor->rozmiar;
        if (IsType<Ludek>(twor))
            rozmiar *= 1.2;

        int wysokosc = rozmiar * wysokosc_jednego / dlugosc_jednego;  // trzeba to gdzieś potem wyciągnąć
        wyglad.setSize(sf::Vector2f(rozmiar * 2, wysokosc * 2));
        wyglad.setOrigin(rozmiar, wysokosc);
        //sf::Color polprzezroczysty = twor->gracz->kolor;
        //polprzezroczysty.a = 128;
        //wyglad.setFillColor(polprzezroczysty);

        int ziarno = ((unsigned int)twor) % 100;
        if (liczba_ramek > 0)
        {
            int ramka_numer = ((clock() * liczba_ramek * 2 / CLOCKS_PER_SEC) + ziarno) % liczba_ramek;
            wyglad.setTexture(animacja_tworu.getSpriteSheet());
            wyglad.setTextureRect(animacja_tworu.getFrame(ramka_numer));
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

        if(rysuj) okno.draw(podpis);

        // nie potrzebne już! rysuj poziom domku
        /*if (IsType<Domek>(twor))
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
        }*/
        // rysuj tarcze ludka
        if (IsType<Ludek>(twor))
        {
            int tarcza = ((Ludek*)twor)->tarcza;
            if (tarcza > 0)
            {
                podpis.setFillColor(sf::Color::White);
                podpis.setCharacterSize(12);
                podpis.setOutlineColor(twor->gracz->kolor);
                podpis.setString(std::to_string(tarcza));
                podpis.setPosition(twor->polozenie.x - 10 * podpis.getString().getSize() / 2, twor->polozenie.y - wysokosc * 1.8);
                if (rysuj) okno.draw(podpis);
            }

            bool lustro = ((Ludek*)twor)->cel->polozenie.x < ((Ludek*)twor)->polozenie.x;
            if (lustro)
                wyglad.setScale(-1, 1);
            else wyglad.setScale(1, 1);
        }

        if (rysuj) okno.draw(wyglad);
    }
}

OznaczaczWyborow::OznaczaczWyborow(MyszDecydent & decydent) : decydent(decydent)
{
}



void OznaczaczWyborow::Wyswietlaj(sf::RenderWindow & okno)
{
    if (decydent.skupiony != nullptr && decydent.skupiony->uid >= 0 && decydent.skupiony->uid <= decydent.skupiony->last_uid&& decydent.skupiony != decydent.wybrany) // TMP TODO
    {
        double wspolczynnik_czas_odznaczenia = 1.6; // moznaby tutaj wyliczyć wielkość okregu w zaleznosc od czasu od ostatniego klikniecia(klikniecia sa private w klasie decydenta)
        double rozmiar = decydent.skupiony->rozmiar * wspolczynnik_czas_odznaczenia;
        sf::CircleShape kolo(rozmiar);
        kolo.setPosition(decydent.skupiony->polozenie.x, decydent.skupiony->polozenie.y);
        kolo.setRadius(rozmiar);
        kolo.setOrigin(rozmiar, rozmiar);
        sf::Color kolor = decydent.skupiony->gracz->kolor;
        kolor.a = 150;
        kolo.setFillColor(kolor);

        okno.draw(kolo);
    }
    if (decydent.wybrany != nullptr)
    {
        double wspolczynnik_czas_odznaczenia = 1.6; // moznaby tutaj wyliczać wielkość okregu w zaleznosc od czasu od ostatniego klikniecia(klikniecia sa private w klasie decydenta)
        double rozmiar = decydent.wybrany->rozmiar * wspolczynnik_czas_odznaczenia;
        sf::CircleShape kolo(rozmiar);
        kolo.setPosition(decydent.wybrany->polozenie.x, decydent.wybrany->polozenie.y);
        kolo.setRadius(rozmiar);
        kolo.setOrigin(rozmiar, rozmiar);
        sf::Color kolor = decydent.wybrany->gracz->kolor;
        kolor.a = 200;
        kolo.setFillColor(kolor);

        okno.draw(kolo);
    }
    if (decydent.kontrola&&decydent.kontrolowany != nullptr)
    {
        auto start_drogi = sf::Vector2f(decydent.kontrolowany->polozenie.x, decydent.kontrolowany->polozenie.y+15);
        auto koniec_drogi = okno.mapPixelToCoords(sf::Mouse::getPosition(okno));
        auto kierunek = koniec_drogi - start_drogi;
        double odleglosc = sqrt(kierunek.x * kierunek.x + kierunek.y * kierunek.y);
        if (odleglosc > decydent.kontrolowany->rozmiar * 1.5){
        rysuj_strzalke(okno, start_drogi, kierunek, decydent.gracz.kolor, 5);
         }
    }

    for (auto& pk : decydent.punkty_kontrolne)
    {
        auto wybrany = pk.first;
        auto docelowy = pk.second;
        auto centrum = sf::Vector2f(wybrany->polozenie.x, wybrany->polozenie.y);
        if (wybrany == docelowy) // ulepszanie automatyczne
        {
            // rysuj strzalke do gory
            auto ulepszanie = sf::Vector2f(wybrany->rozmiar * 1.2, 0);
            auto polowa_dlugosci = sf::Vector2f(0, 15);
            auto cykl = polowa_dlugosci * abs(cykl_czasowy() - 0.5f);

            rysuj_strzalke(okno, centrum + ulepszanie + polowa_dlugosci - cykl, -polowa_dlugosci * 2.0f, decydent.gracz.kolor, 3);
        }
        else // wysylanie automatyczne
        {
            auto start_drogi = sf::Vector2f(wybrany->polozenie.x, wybrany->polozenie.y+15 );
            auto koniec_drogi = sf::Vector2f(docelowy->polozenie.x, docelowy->polozenie.y+15);
            auto kierunek = koniec_drogi - start_drogi;
            auto cykl = 0.25f * abs(cykl_czasowy() - 0.5f);

            rysuj_strzalke(okno, start_drogi, kierunek * (0.60f + cykl), decydent.gracz.kolor, 3);
        }
    }
}

Domek* OznaczaczWyborow::WybranyDomek()
{
    return decydent.wybrany;
}
float OznaczaczWyborow::cykl_czasowy()
{
    int czas_cyklu = 1;
    float sekundy = (clock() / (float)CLOCKS_PER_SEC);
    float reszta = sekundy - (int)(sekundy / czas_cyklu)*czas_cyklu;
    return reszta / (float)czas_cyklu;
}

void OznaczaczWyborow::rysuj_strzalke(sf::RenderWindow & okno, sf::Vector2f start, sf::Vector2f kierunek, sf::Color color, float grubosc)
{
    thor::Arrow cien(start, kierunek, sf::Color::Black, grubosc + 1);
    okno.draw(cien);

    thor::Arrow strzala(start, kierunek, color, grubosc);
    okno.draw(strzala);
}
