#include "misja.h"
#include "gui.h"

#include "ext_string.h"
#include <SFML/Audio.hpp>

#include "easylogging++.h"

#include <cstdlib>
#include <ctime>

MisjaUstawienia::MisjaUstawienia() : ile_kto_wygranych(16) {}

int MisjaUstawienia::Zwyciezca()
{
    int res = -1;
    for (int i = 0; i < ile_kto_wygranych.size(); i++)
    {
        if (ile_kto_wygranych[i] >= do_ilu_wygranych)
            return i;
    }
    return res;
}

bool MisjaUstawienia::JedenGracz()
{
    return nazwy_graczow.size() <= 1;
}

void MisjaUstawienia::WypiszRanking()
{
    vector<pair<int,string>> stan;
    for (int i = 0; i < min(ile_kto_wygranych.size(), nazwy_graczow.size()); i++)
    {
        stan.push_back(make_pair(ile_kto_wygranych[i], nazwy_graczow[i]));
    }
    sort(stan.begin(), stan.end(), greater<pair<int, string>>());
    printf("Ranking:\n");
    for (int i = 0; i < stan.size(); i++)
    {
        printf("%s - %d\n", stan[i].second.c_str(), stan[i].first);
    }
}

vector<string> wczytaj_liste_plansz(string folder)
{
    return get_all_names_within_folder(folder);
}

vector<string> wczytaj_liste_folderow(string folder)
{
    return get_all_names_within_folder(folder,1);
}

MisjaUstawienia wczytaj_meta(string sciezka)
{
    MisjaUstawienia res;
    res.komputery.clear();

    ifstream plikmapa;
    plikmapa.open(sciezka);
    int liczba_domkow, numer_domku, liczba_parametrow;
    string parametr;
    plikmapa >> liczba_domkow;

    // wyznacz liczbę graczy na apie
    while (plikmapa.good())
    {
        plikmapa >> parametr;
        if (parametr == "gracz")
        {
            int num;
            plikmapa >> num;
            res.komputery.push_back(num);
        }
    }

    sort(res.komputery.begin(), res.komputery.end());
    res.komputery.erase(unique(res.komputery.begin(), res.komputery.end()), res.komputery.end());
    res.komputery.erase(res.komputery.begin()); // usun pierwszego
    res.ile_kto_wygranych = vector<int>(13);

    plikmapa.close();

    return res;
}

Rozgrywka zwarcie_rozgrywka(string sciezka)
{
    Rozgrywka gra;
    //gracze
    sf::Color kolory[] = { sf::Color(128,128,128) , sf::Color(255,0,0), sf::Color(0,0,255), sf::Color(0,255,0),      // 0 - szary, 1 - czerwony, 2 - niebieski, 3 - zielony
    sf::Color(255,255,0) , sf::Color(255,128,0), sf::Color(0,255,255), sf::Color(127,0,255),                              // 4 - zolty, 5 - pomaranczowy, 6 - jasnoniebieski, 7- fioletowy
    sf::Color(255,0,255) , sf::Color(102,0,51), sf::Color(102,102,0), sf::Color(51,25,0),                        // 8 - rozowy, 9 - bordowy, 10 - ciemnozloty, 11 - brazowy
    sf::Color(255,255,255), };                                                                                                                             // 12 - bialy

    for (int a = 0; a <= 12 ; a++)
    {
        gra.gracze.push_back(Gracz());
        Gracz& gracz = gra.gracze.back();
        gracz.numer = a;
        if (a == 0)gracz.aktywny = false;
        else if (a == 1)gracz.nazwa = "GRACZ";
        gracz.kolor = kolory[a];
    }

    gra.liczba_aktywnych_graczy = 12;
    //domki
    ifstream plikmapa;
    plikmapa.open(sciezka);
    int liczba_domkow, numer_domku, liczba_parametrow;
    string parametr;
    plikmapa >> liczba_domkow;

    //KOORDYNATY , DROGI , GRACZ

    map<int, Domek*> domki_po_numerze;
    vector<std::pair<int,int>> numery_domkow;
    for (int a = 0; a < liczba_domkow; a++)
        {
        plikmapa >> numer_domku >> liczba_parametrow;
        gra.domki.push_back(Domek());
        Domek& domek = gra.domki.back();
        domki_po_numerze[numer_domku] = &domek;
        for (int b = 0; b < liczba_parametrow; b++)
            {
            plikmapa >> parametr;
            if (parametr == "koordynaty")
                {
                double x, y;
                plikmapa >> x >> y;
                domek.polozenie = {x,y};
                }
            else if (parametr == "drogi")
                {
                int ile,numer;
                plikmapa >> ile;
                for (int c = 0; c < ile; c++)
                    {
                    plikmapa >> numer;
                    numery_domkow.push_back(make_pair(numer_domku, numer));
                    }
                }
            else if (parametr == "gracz")
                {
                int numer;
                plikmapa >> numer;
                domek.gracz = &gra.Graczu(numer);
                gra.Graczu(numer).liczba_tworow++;
                }
            else if(parametr == "typ")
                {
                string znak;
                plikmapa >> znak;
                if (znak == "zamek"||znak=="fort")domek.typdomku = TypDomku::kFort;
                else if (znak == "wieza")domek.typdomku = TypDomku::kWieza;
                else if (znak == "kuznia"||znak=="zbrojownia")domek.typdomku = TypDomku::kZbrojownia;
                else if (znak == "stajnia")domek.typdomku = TypDomku::kStajnia;
                else if (znak == "pole")domek.typdomku = TypDomku::kPole;
                }
            else if (parametr == "bez_przebudowy")
                {
                domek.przebudowa = false;
                }
            else if (parametr == "bez_ulepszania")
                {
                    domek.ulepszanie = false;
                }
            else if (parametr == "liczebnosc")
                {
                    int ile_ludkow;
                    plikmapa >> ile_ludkow;
                    if (domek.max_liczebnosc == -1)domek.max_liczebnosc = 100;
                    gra.ZmienLiczebnosc(domek, ile_ludkow);
                }
            else if (parametr == "poziom")
                {
                    int lev;
                    plikmapa >> lev;
                    gra.ZmienPoziom(domek, lev);
                }
            }
        domek.produkcja = 2;
        if(domek.max_liczebnosc==-1)domek.max_liczebnosc = 100;
        domek.wyglad = Wyglad::kMiasto;
        if (domek.gracz == nullptr)
        {
            domek.gracz = &gra.Graczu(0);
            if(domek.liczebnosc==-1)gra.ZmienLiczebnosc(domek, 25);
        }
        else if(domek.liczebnosc==-1)gra.ZmienLiczebnosc(domek, 50);
        }
    for (auto para : numery_domkow)
    {
        Domek   *wksslask1,*wksslask2;
        wksslask1 = domki_po_numerze[para.first];
        wksslask2 = domki_po_numerze[para.second];
        wksslask1->drogi.push_back(wksslask2);
        wksslask2->drogi.push_back(wksslask1);
    }

    for (auto& gracz : gra.gracze) if (gracz.numer >= 1)
    {
        gracz.istotny = gracz.liczba_tworow > 0;
        gracz.aktywny = gracz.liczba_tworow > 0;
        if (!gracz.aktywny)
            gra.liczba_aktywnych_graczy--;
    }
    
    plikmapa.close();
    return gra;
}

string ranking_widget_id(int instance, int gracz, string sufix)
{
    return "Ins-" + to_string(instance) + "-Inter-Gracz-" + to_string(gracz) + "-" + sufix;
}

void interfejs_wybrany_ustaw(shared_ptr<sfg::Window> interfejs, Rozgrywka& rozgrywka, Wyswietlacz& wyswietlacz, Domek* wybrany, Twor* skupiony)
{
    auto wyglad = std::static_pointer_cast<sfg::Canvas>(interfejs->GetWidgetById("Wybrany-wyglad"));
    auto info_poziom = std::static_pointer_cast<sfg::Label>(interfejs->GetWidgetById("Wybrany-poziom"));
    auto info_poziom_etykieta = std::static_pointer_cast<sfg::Label>(interfejs->GetWidgetById("Wybrany-etk-poziom"));
    auto info_zapelnienie = std::static_pointer_cast<sfg::Label>(interfejs->GetWidgetById("Wybrany-zapelnienie"));
    auto info_zapelnienie_etykieta = std::static_pointer_cast<sfg::Label>(interfejs->GetWidgetById("Wybrany-etk-zapelnienie"));
    auto info_ulepsz = std::static_pointer_cast<sfg::Label>(interfejs->GetWidgetById("Wybrany-ulepszenie"));
    auto info_ulepsz_etykieta = std::static_pointer_cast<sfg::Label>(interfejs->GetWidgetById("Wybrany-etk-ulepszenie"));
    auto info_atak = std::static_pointer_cast<sfg::Label>(interfejs->GetWidgetById("Wybrany-atak"));
    auto info_atak_etykieta = std::static_pointer_cast<sfg::Label>(interfejs->GetWidgetById("Wybrany-etk-atak"));
    auto info_obrona = std::static_pointer_cast<sfg::Label>(interfejs->GetWidgetById("Wybrany-obrona"));
    auto info_obrona_etykieta = std::static_pointer_cast<sfg::Label>(interfejs->GetWidgetById("Wybrany-etk-obrona"));
    
	auto do_pokazania = skupiony != nullptr ? skupiony : wybrany;
    if (do_pokazania != nullptr)
    {
        auto& obrazek = wyswietlacz.PobierzWyglad(do_pokazania);
        obrazek.setPosition(50, 50);
        obrazek.setSize(sf::Vector2f(100, 100));
        obrazek.setOrigin(sf::Vector2f(50, 50));
        wyglad->Clear(do_pokazania->gracz->kolor);
        wyglad->Draw(obrazek);

		if (IsType<Domek>(do_pokazania))
		{
			auto pokaz_domek = (Domek*)do_pokazania;
			info_poziom_etykieta->SetText(L"Poziom:");
			info_poziom->SetText(string_format("%d", pokaz_domek->poziom));
			info_zapelnienie_etykieta->SetText(L"Zapełnienie:");
			if (pokaz_domek->typdomku != TypDomku::kPole)info_zapelnienie->SetText(to_wstring(100 * (int)pokaz_domek->liczebnosc / (int)pokaz_domek->max_liczebnosc) + L"%");
			else info_zapelnienie->SetText("---");
			info_ulepsz_etykieta->SetText(L"Koszt ulepszenia:");
			if (pokaz_domek->poziom < 5)
				info_ulepsz->SetText(string_format("%d", pokaz_domek->max_liczebnosc / 2));
			else
				info_ulepsz->SetText(" --- ");
			info_atak_etykieta->SetText(L"Atak:");
			info_atak->SetText(string_format("%d", (int)rozgrywka.PoliczAtakDomku(*pokaz_domek)));
			info_obrona_etykieta->SetText(L"Obrona:");
			info_obrona->SetText(string_format("%d", (int)rozgrywka.PoliczObroneDomku(*pokaz_domek)));
		}
		else
		{
			auto pokaz_ludek = (Ludek*)do_pokazania;
			info_poziom_etykieta->SetText(L"Szybkość:");
			info_poziom->SetText(string_format("%d", pokaz_ludek->szybkosc_ludka));
			info_zapelnienie_etykieta->SetText(L"Tarcza:");
			info_zapelnienie->SetText(string_format("%d", pokaz_ludek->tarcza));
		}
    }
    else
    {
        wyglad->Clear(sf::Color::Transparent);

        info_poziom->SetText("");
        info_poziom_etykieta->SetText("");
        info_zapelnienie->SetText("");
        info_zapelnienie_etykieta->SetText("");
        info_ulepsz->SetText("");
        info_ulepsz_etykieta->SetText("");
        info_atak->SetText("");
        info_atak_etykieta->SetText("");
        info_obrona->SetText("");
        info_obrona_etykieta->SetText("");
    }
}

shared_ptr<sfg::Table> interfejs_wybrany()
{
    auto table = sfg::Table::Create();

    auto wyglad = sfg::Canvas::Create();
    wyglad->SetId("Wybrany-wyglad");
    wyglad->SetRequisition(sf::Vector2f(100, 100));

    auto info_poziom_etykieta = sfg::Label::Create();
    info_poziom_etykieta->SetId("Wybrany-etk-poziom");
    info_poziom_etykieta->SetClass("WybranyTekst");
    info_poziom_etykieta->SetAlignment(sf::Vector2f(0.0, 0.5));
    auto info_poziom = sfg::Label::Create();
    info_poziom->SetId("Wybrany-poziom");
    info_poziom->SetClass("WybranyTekst");
    info_poziom->SetAlignment(sf::Vector2f(1.0, 0.5));

    auto info_zapelnienie_etykieta = sfg::Label::Create();
    info_zapelnienie_etykieta->SetId("Wybrany-etk-zapelnienie");
    info_zapelnienie_etykieta->SetClass("WybranyTekst");
    info_zapelnienie_etykieta->SetAlignment(sf::Vector2f(0.0, 0.5));
    auto info_zapelnienie = sfg::Label::Create();
    info_zapelnienie->SetId("Wybrany-zapelnienie");
    info_zapelnienie->SetClass("WybranyTekst");
    info_zapelnienie->SetAlignment(sf::Vector2f(1.0, 0.5));

    auto info_ulepsz_etykieta = sfg::Label::Create();
    info_ulepsz_etykieta->SetId("Wybrany-etk-ulepszenie");
    info_ulepsz_etykieta->SetClass("WybranyTekst");
    info_ulepsz_etykieta->SetAlignment(sf::Vector2f(0.0, 0.5));
    auto info_ulepsz = sfg::Label::Create();
    info_ulepsz->SetId("Wybrany-ulepszenie");
    info_ulepsz->SetClass("WybranyTekst");
    info_ulepsz->SetAlignment(sf::Vector2f(1.0, 0.5));

    auto info_atak_etykieta = sfg::Label::Create();
    info_atak_etykieta->SetId("Wybrany-etk-atak");
    info_atak_etykieta->SetClass("WybranyTekst");
    info_atak_etykieta->SetAlignment(sf::Vector2f(0.0, 0.5));
    auto info_atak = sfg::Label::Create();
    info_atak->SetId("Wybrany-atak");
    info_atak->SetClass("WybranyTekst");
    info_atak->SetAlignment(sf::Vector2f(1.0, 0.5));

    auto info_obrona_etykieta = sfg::Label::Create();
    info_obrona_etykieta->SetId("Wybrany-etk-obrona");
    info_obrona_etykieta->SetClass("WybranyTekst");
    info_obrona_etykieta->SetAlignment(sf::Vector2f(0.0, 0.5));
    auto info_obrona = sfg::Label::Create();
    info_obrona->SetId("Wybrany-obrona");
    info_obrona->SetClass("WybranyTekst");
    info_obrona->SetAlignment(sf::Vector2f(1.0, 0.5));

    table->Attach(wyglad, sf::Rect<sf::Uint32>(0, 0, 1, 5), 0, 3, sf::Vector2f(10, 0));
    table->Attach(info_poziom_etykieta, sf::Rect<sf::Uint32>(1, 0, 1, 1), 0);
    table->Attach(info_zapelnienie_etykieta, sf::Rect<sf::Uint32>(1, 1, 1, 1), 0);
    table->Attach(info_ulepsz_etykieta, sf::Rect<sf::Uint32>(1, 2, 1, 1), 0);
    table->Attach(info_atak_etykieta, sf::Rect<sf::Uint32>(1, 3, 1, 1), 0);
    table->Attach(info_obrona_etykieta, sf::Rect<sf::Uint32>(1, 4, 1, 1), 0);
    table->Attach(info_poziom, sf::Rect<sf::Uint32>(2, 0, 1, 1), 3);
    table->Attach(info_zapelnienie, sf::Rect<sf::Uint32>(2, 1, 1, 1), 3);
    table->Attach(info_ulepsz, sf::Rect<sf::Uint32>(2, 2, 1, 1), 3);
    table->Attach(info_atak, sf::Rect<sf::Uint32>(2, 3, 1, 1), 3);
    table->Attach(info_obrona, sf::Rect<sf::Uint32>(2, 4, 1, 1), 3);

    table->SetRequisition(sf::Vector2f(300, 100));

    return table;
}

shared_ptr<sfg::Table> interfejs_ranking(MisjaUstawienia &stan, Rozgrywka& rozgrywka, int instance)
{
    auto table = sfg::Table::Create();

    auto gracze = rozgrywka.gracze;
    gracze.sort([stan](const Gracz & a, const Gracz & b) -> bool
    {
        return stan.ile_kto_wygranych[a.numer] > stan.ile_kto_wygranych[b.numer];
    });

    int i = 0;

    // dorzuc nagłowek mały
    auto wartosc = sfg::Label::Create(stan.do_ilu_wygranych <= 1 ? "" : "gra do " + to_string(stan.do_ilu_wygranych));
    wartosc->SetClass("GridOpis");
    auto nazwa = sfg::Label::Create("nazwa");
    nazwa->SetClass("GridOpis");
    auto ludki = sfg::Label::Create("ludki");
    ludki->SetClass("GridOpis");
    auto produkcja = sfg::Label::Create("prod.");
    produkcja->SetClass("GridOpis");
    auto modernizacja = sfg::Label::Create("ulep.");
    modernizacja->SetClass("GridOpis");
    auto szybkosc = sfg::Label::Create("szyb.");
    szybkosc->SetClass("GridOpis");

    table->Attach(wartosc, sf::Rect<sf::Uint32>(0, i, 1, 1), 3, 3, sf::Vector2f(0, 3));
    table->Attach(nazwa, sf::Rect<sf::Uint32>(1, i, 1, 1));
    table->Attach(ludki, sf::Rect<sf::Uint32>(2, i, 1, 1));
    table->Attach(produkcja, sf::Rect<sf::Uint32>(3, i, 1, 1));
    table->Attach(modernizacja, sf::Rect<sf::Uint32>(4, i, 1, 1));
    table->Attach(szybkosc, sf::Rect<sf::Uint32>(5, i, 1, 1));

    i++;

    for (auto& gracz : gracze)
    {
        auto nr = gracz.numer;
        if (gracz.istotny)
        {
            auto graczId = ranking_widget_id(instance, nr, "");
            auto graczLudkiId = ranking_widget_id(instance, nr, "sila");
            auto graczProdukcjaId = ranking_widget_id(instance, nr, "produkcja");
            auto graczModernizacjaId = ranking_widget_id(instance, nr, "modernizacja");
            auto graczSzybkoscId = ranking_widget_id(instance, nr, "szybkosc");

            // to zajmuje jakos duzo czasu, nie wiem czemu!
            GUI::aplikacja().pulpit.SetProperty<sf::Color>("Button#" + graczId, "BackgroundColor", gracz.kolor);
            GUI::aplikacja().pulpit.SetProperty("Button#" + graczId, "FontSize", 26);
            GUI::aplikacja().pulpit.SetProperty("Label#" + graczId, "FontSize", 28);
            GUI::aplikacja().pulpit.SetProperty("Label#" + graczId, "FontName", "Grafika/BuxtonSketch.ttf");
            GUI::aplikacja().pulpit.SetProperty("Label#" + graczProdukcjaId, "Color", gracz.kolor);
            GUI::aplikacja().pulpit.SetProperty("Label#" + graczProdukcjaId, "FontSize", 28);
            GUI::aplikacja().pulpit.SetProperty("Label#" + graczModernizacjaId, "Color", sf::Color::Yellow);
            GUI::aplikacja().pulpit.SetProperty("Label#" + graczModernizacjaId, "FontSize", 28);
            GUI::aplikacja().pulpit.SetProperty("Label#" + graczSzybkoscId, "Color", sf::Color::Black);
            GUI::aplikacja().pulpit.SetProperty("Label#" + graczSzybkoscId, "FontSize", 28);
            GUI::aplikacja().pulpit.SetProperty("Label#" + graczLudkiId, "FontName", "Grafika/waltographUI.ttf");
            GUI::aplikacja().pulpit.SetProperty("Label#" + graczLudkiId, "FontSize", 28);
            if (stan.Zwyciezca() == nr)
            {
                GUI::aplikacja().pulpit.SetProperty("Button#" + graczId, "FontSize", 80.f);
                GUI::aplikacja().pulpit.SetProperty("Label#" + graczId, "FontSize", 80.f);
            }

            auto wartosc = sfg::Button::Create(to_string(stan.ile_kto_wygranych[nr]));
            wartosc->SetId(graczId);
            auto nazwa = sfg::Label::Create(gracz.nazwa);
            nazwa->SetAlignment(sf::Vector2f(0.5, 0.5));
            nazwa->SetId(graczId);

            auto sila_gracza = rozgrywka.SilaGracza(nr);
            auto ludki = sfg::Label::Create(to_string(std::get<0>(sila_gracza)));
            ludki->SetAlignment(sf::Vector2f(0.5, 0.5));
            ludki->SetId(graczLudkiId);
            auto produkcja = sfg::Label::Create(to_string(std::get<1>(sila_gracza)));
            produkcja->SetAlignment(sf::Vector2f(0.5, 0.5));
            produkcja->SetId(graczProdukcjaId);
            auto modernizacja = sfg::Label::Create(to_string(std::get<2>(sila_gracza)));
            modernizacja->SetAlignment(sf::Vector2f(0.5, 0.5));
            modernizacja->SetId(graczModernizacjaId);
            auto szybkosc = sfg::Label::Create(to_string(std::get<3>(sila_gracza)));
            szybkosc->SetAlignment(sf::Vector2f(0.5, 0.5));
            szybkosc->SetId(graczSzybkoscId);

            table->Attach(wartosc, sf::Rect<sf::Uint32>(0, i, 1, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL);// , sf::Vector2f(10.f, 10.f));
            table->Attach(nazwa, sf::Rect<sf::Uint32>(1, i, 1, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL);//, sf::Vector2f(10.f, 10.f));
            table->Attach(ludki, sf::Rect<sf::Uint32>(2, i, 1, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL);//, sf::Vector2f(10.f, 10.f));
            table->Attach(produkcja, sf::Rect<sf::Uint32>(3, i, 1, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL);//, sf::Vector2f(10.f, 10.f));
            table->Attach(modernizacja, sf::Rect<sf::Uint32>(4, i, 1, 1) , sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL);//, sf::Vector2f(10.f, 10.f));
            table->Attach(szybkosc, sf::Rect<sf::Uint32>(5, i, 1, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL);//, sf::Vector2f(10.f, 10.f));
            i++;
        }
    }

    return table;
}

shared_ptr<sfg::Window> interfejs_rozgrywki(shared_ptr<sfg::Window> interfejs, MisjaUstawienia &stan, Rozgrywka& rozgrywka, Wyswietlacz& wyswietlacz, Domek* wybrany, Twor* skupiony)
{
    
    if (interfejs == nullptr)
    {
        if (stan.do_ilu_wygranych > 0)
        {
            interfejs = sfg::Window::Create(sfg::Window::Style::BACKGROUND | sfg::Window::Style::SHADOW);
            interfejs->SetTitle("Mecz do " + to_string(stan.do_ilu_wygranych) + " wygranych");
            interfejs->SetRequisition(sf::Vector2f(140, 0));

            auto ranking = interfejs_ranking(stan, rozgrywka, 1);

            auto pomoc = sfg::Image::Create();
            sf::Image pomoc_obraz;
            if (pomoc_obraz.loadFromFile("Grafika\\rycerze_hd\\info.png"))
                pomoc->SetImage(pomoc_obraz);
            
            auto info = interfejs_wybrany();
            interfejs_wybrany_ustaw(interfejs, rozgrywka, wyswietlacz, wybrany, skupiony);

            auto tabela_interfejsu = sfg::Table::Create();
            tabela_interfejsu->Attach(ranking, sf::Rect<sf::Uint32>(0, 0, 1, 1));
            tabela_interfejsu->Attach(pomoc, sf::Rect<sf::Uint32>(1, 0, 1, 1));
            tabela_interfejsu->Attach(info, sf::Rect<sf::Uint32>(2, 0, 1, 1));

            GUI::aplikacja().pulpit.Add(interfejs);
            interfejs->Add(tabela_interfejsu);
            GUI::aplikacja().bottom_left_window(interfejs);
        }
    }
    else
    {
        for (auto& gracz : rozgrywka.gracze)
        {
            auto nr = gracz.numer;
            if (gracz.istotny)
            {
                auto sila_gracza = rozgrywka.SilaGracza(nr);
                auto sila_label = std::static_pointer_cast<sfg::Label>(interfejs->GetWidgetById(ranking_widget_id(1, nr, "sila")));
                sila_label->SetText((to_string(std::get<0>(sila_gracza))));

                auto produkcja_label = std::static_pointer_cast<sfg::Label>(interfejs->GetWidgetById(ranking_widget_id(1, nr, "produkcja")));
                produkcja_label->SetText((to_string(std::get<1>(sila_gracza))));

                auto modernizacja_label = std::static_pointer_cast<sfg::Label>(interfejs->GetWidgetById(ranking_widget_id(1, nr, "modernizacja")));
                modernizacja_label->SetText((to_string(std::get<2>(sila_gracza))));

                auto szybkosc_label = std::static_pointer_cast<sfg::Label>(interfejs->GetWidgetById(ranking_widget_id(1, nr, "szybkosc")));
                szybkosc_label->SetText((to_string(std::get<3>(sila_gracza))));
            }
        }

        interfejs_wybrany_ustaw(interfejs, rozgrywka, wyswietlacz, wybrany, skupiony);
    }
    return interfejs;
}

void odliczanie()
{
    for (int a = 5; a >= 0; a--)
    {
        if (a > 0)
            Sleep(1500);
        else
            Sleep(800);
    }
}

void odliczanie(Wyswietlacz& wyswietlacz, sf::View widok, std::shared_ptr<sfg::Window> gui_pasek)
{
    auto okno = sfg::Window::Create(sfg::Window::Style::BACKGROUND | sfg::Window::Style::SHADOW);
    okno->SetRequisition(sf::Vector2f(500, 300));
    GUI::aplikacja().center_window(okno);

    auto odliczanie_etykieta = sfg::Label::Create("");
    odliczanie_etykieta->SetId("Alarm");
    okno->Add(odliczanie_etykieta);
    GUI::aplikacja().pulpit.Add(okno);

    sf::SoundBuffer pikPikBuffer;
    pikPikBuffer.loadFromFile("Muzyka\\Boom.ogg");

    sf::SoundBuffer pukBuffer;
    pukBuffer.loadFromFile("Muzyka\\Bam.ogg");

    sf::Sound pikPik(pikPikBuffer);
    pikPik.setVolume(30);

    sf::Sound puk(pukBuffer);
    puk.setPitch(2);

    GUI::aplikacja().show_bottom_gui(widok, gui_pasek);
    for (int a = 3; a >= 0; a--)
    {
        GUI::aplikacja().okno.clear();

        wyswietlacz.WyswietlTlo(GUI::aplikacja().okno);
        if (a>0) odliczanie_etykieta->SetText(std::to_string(a));
        else odliczanie_etykieta->SetText("RUSZAJ!");
        wyswietlacz.Wyswietlaj(GUI::aplikacja().okno);

        GUI::aplikacja().show_bottom_gui(widok, gui_pasek);
        GUI::aplikacja().okno.display();

        if (a > 0)
        {
            if (GUI::aplikacja().dzwieki_glosnosc)
                pikPik.play();
            Sleep(1500);
        }
        else
        {
            if (GUI::aplikacja().dzwieki_glosnosc)
                puk.play();
            Sleep(800);
        }
    }

    GUI::aplikacja().pulpit.Remove(okno);
}

void zakonczenie_gry(Gracz& gracz_wygrany, int grajacy)
{
    auto okno = sfg::Window::Create(sfg::Window::Style::BACKGROUND | sfg::Window::Style::SHADOW);
    okno->SetRequisition(sf::Vector2f(800, 300));

    auto komunikat_koncowy = sfg::Label::Create("");
    komunikat_koncowy->SetId("Naglowek");
    komunikat_koncowy->SetAlignment(sf::Vector2f(0.5, 0.5));
    okno->Add(komunikat_koncowy);
    sf::SoundBuffer oklaskiBuffer;
    oklaskiBuffer.loadFromFile("Muzyka\\Oklaski.ogg");
    sf::Sound oklaski(oklaskiBuffer);

    if (gracz_wygrany.numer == grajacy)
    {
        if (GUI::aplikacja().dzwieki_glosnosc)
            oklaski.play();
        komunikat_koncowy->SetText("GRATULACJE DLA GRACZA:\n" + gracz_wygrany.nazwa);
    }
    else
        komunikat_koncowy->SetText("TY LESZCZU ZWYCIEZYL:\n" + gracz_wygrany.nazwa);

    GUI::aplikacja().show_and_wait_for_anything(okno);
}

void zakonczenie_meczu(MisjaUstawienia &stan, Rozgrywka& rozgrywka)
{
    auto okno = sfg::Window::Create(sfg::Window::Style::BACKGROUND | sfg::Window::Style::SHADOW);
    okno->SetRequisition(sf::Vector2f(800, 0));

    auto komunikat_koncowy = sfg::Label::Create("OSTATECZNY RANKING:\n");
    komunikat_koncowy->SetId("Naglowek");
    komunikat_koncowy->SetAlignment(sf::Vector2f(0.5, 0.5));

    auto ranking = interfejs_ranking(stan, rozgrywka, 2);
    
    auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 30.0f);
    box->Pack(komunikat_koncowy);
    box->Pack(ranking);
    
    okno->Add(box);
    GUI::aplikacja().show_and_wait_for_anything(okno);
}

sf::View wysrodkowany_widok(list<Domek> &domki, int bottom_space)
{
    auto minimalny_X = 10000.0, maksymalny_X = 0.0;
    auto minimalny_Y = 10000.0, maksymalny_Y = 0.0;

    for (auto& dom : domki)
    {
        minimalny_X = min(minimalny_X, (double)dom.polozenie.x);
        maksymalny_X = max(maksymalny_X, (double)dom.polozenie.x);

        minimalny_Y = min(minimalny_Y, (double)dom.polozenie.y);
        maksymalny_Y = max(maksymalny_Y, (double)dom.polozenie.y);
    }
    
    auto powiekszenie_x = min(minimalny_X, 100.0);
    minimalny_X -= powiekszenie_x;
    maksymalny_X += powiekszenie_x;

    auto powiekszenie_y = min(minimalny_Y, 100.0);
    minimalny_Y -= powiekszenie_y;
    maksymalny_Y += powiekszenie_y;

    auto okno_size = GUI::aplikacja().okno.getSize();

    double stosunek = (double) okno_size.x / (okno_size.y - bottom_space);
    auto dlugosc_Y = maksymalny_Y - minimalny_Y;
    auto dlugosc_X = maksymalny_X - minimalny_X;

    auto dlugosc_X_stosunku = stosunek * dlugosc_Y;
    auto dlugosc_X_z_prop = max(dlugosc_X_stosunku, dlugosc_X);
    auto dlugosc_Y_z_prop = dlugosc_X_z_prop / stosunek;

    minimalny_X += (dlugosc_X - dlugosc_X_z_prop) / 2;

    return sf::View(sf::FloatRect(minimalny_X, minimalny_Y, dlugosc_X_z_prop, dlugosc_Y_z_prop));
}

int misja(MisjaUstawienia& misja_ustawienia, Ruszacz& ruszacz)
{
    string sciezka = join(misja_ustawienia.grupa, misja_ustawienia.nazwa);
    string trudnosc = misja_ustawienia.trudnosc;
    double predkosc = misja_ustawienia.szybkosc;
    int nr_gracza = misja_ustawienia.nr_gracza;
    bool przyspieszenie_bez_gracza = false;

    sf::RenderWindow& window = GUI::aplikacja().okno;

    bool to_serwer = nr_gracza == 0;

    if (to_serwer)
    {
        window.setVisible(false);
        GUI::aplikacja().dzwieki_glosnosc = 0;
    }

    // tworzymy rozgrywke
    Rozgrywka rozgrywka = zwarcie_rozgrywka(sciezka);
    rozgrywka.oszustwa = misja_ustawienia.oszustwa;
    rozgrywka.walka_w_polu = misja_ustawienia.walka_w_polu;
    rozgrywka.punkty_kontrolne = misja_ustawienia.punkty_kontrolne;
    // przygotowujemy dzialaczy
    Wyswietlacz wyswietlacz(rozgrywka);
    if (!to_serwer)
        wyswietlacz.Zaladuj(misja_ustawienia.skorka);
    MyszDecydent myszkaGracza(window, rozgrywka, rozgrywka.Graczu(nr_gracza));
    OznaczaczWyborow ruchGracza(myszkaGracza);
    Muzykant muzykant(rozgrywka);
    muzykant.Zaladuj(misja_ustawienia.skorka);

    //ZMIEN NAZWY GRACZ�W
    if (misja_ustawienia.nazwy_graczow.size())
    {
        for (int i = 0; i < misja_ustawienia.nazwy_graczow.size(); i++)
        {
            rozgrywka.Graczu(i).nazwa = misja_ustawienia.nazwy_graczow[i];
        }
    }
    
    //KOMPUTEROWIE
    vector<Komputer*> kompiutery;
    srand(time(NULL));
    for (auto nr : misja_ustawienia.komputery)
    {
        //NADANIE NAZWY CZYLI ROWNIEZ TRYBU GRY
		if(misja_ustawienia.trudnosc==poziomy_trudnosci[1])
			switch (std::rand() % 4)
			{
				case 0:
				{
					rozgrywka.Graczu(nr).nazwa = "Kapuniak";
					kompiutery.emplace_back(new KomputerSilver(rozgrywka, rozgrywka.Graczu(nr), misja_ustawienia.szybkosc, 'K'));
					break;
				}
				case 1:
				{
					rozgrywka.Graczu(nr).nazwa = "Grubas";
					kompiutery.emplace_back(new KomputerSilver(rozgrywka, rozgrywka.Graczu(nr), misja_ustawienia.szybkosc, 'G'));
					break;
				}
				case 2:
				{
					rozgrywka.Graczu(nr).nazwa = "Alfa";
					kompiutery.emplace_back(new KomputerSilver(rozgrywka, rozgrywka.Graczu(nr), misja_ustawienia.szybkosc, 'A'));
					break;
				}
            }
		else {
			rozgrywka.Graczu(nr).nazwa = "Przeciwnik";
			kompiutery.emplace_back(new Komputer(rozgrywka, rozgrywka.Graczu(nr), misja_ustawienia.szybkosc, 'P'));
		}
    }

    bool gotowe_do_rozpoczecia = false;

    ruszacz.rozgrywka = &rozgrywka;
    ruszacz.szybkosc *= predkosc;
    
    shared_ptr<sfg::Window> interfejs;
    sf::View view;
    if (!to_serwer)
    {
        interfejs = interfejs_rozgrywki(nullptr, misja_ustawienia, rozgrywka, wyswietlacz, nullptr, nullptr);
        if (interfejs == nullptr)
        {
            LOG(WARNING) << "Nie utworzyl sie interfejs." << "Liczba domków=" << rozgrywka.domki.size();
            gotowe_do_rozpoczecia = false; 
        }
        else
        {
            view = wysrodkowany_widok(rozgrywka.domki, interfejs->GetAllocation().height);
            window.setView(view);
            odliczanie(wyswietlacz, view, interfejs);
            gotowe_do_rozpoczecia = true;
        }
    }
    else
    {
        odliczanie();
        gotowe_do_rozpoczecia = true;
    }

    if (!GUI::aplikacja().dzwieki_glosnosc)
        muzykant.wyciszony = true;
    
    muzykant.Przygrywaj();

    // Czasomierz i FPS
    auto czasomierz = clock();
    double czas;
    double fpsy;
    int ostatni_pokaz_fps = 0;

    // APM
    long long akcje = 0;
    double start_gry = clock();
    double czas_gry = 0;

    bool trwa_gra = true;

    if (gotowe_do_rozpoczecia)
    while (trwa_gra)
    {
        sf::Event event;

        muzykant.NoGraj();
        myszkaGracza.Przetworz(); // puste
        while (window.pollEvent(event))
        {
            GUI::aplikacja().pulpit.HandleEvent(event);
            myszkaGracza.Przetworz(event);
            if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::KeyReleased)akcje++;
            switch (event.type)
            {
            case sf::Event::KeyReleased:
                switch (event.key.code)
                {
                case sf::Keyboard::Escape:
                    // nie startuj kolejnych gier
                    misja_ustawienia.ile_kto_wygranych[0] = 100;
                    trwa_gra = false;
                    break;
                case sf::Keyboard::F3:
                    wyswietlacz.ZaladujInne();
                    break;
                }
                break;
            }
            if (event.type == sf::Event::Closed)
            {
                throw std::exception("okno zamkniete");
            }
        }
        ///FPSY
        czas = (double)(clock() - czasomierz) / CLOCKS_PER_SEC;
        fpsy = (int)(1 / czas + 0.5);

        if (czas_gry - ostatni_pokaz_fps > 2 && GUI::aplikacja().ini.GetBoolean("przelaczniki", "fps", false))
        {
            printf("FPS: %d\n", fpsy);
            LOG(INFO) << "FPS: " << fpsy;
            ostatni_pokaz_fps = czas_gry;
        }

        czasomierz = clock();
        auto& ruchy = myszkaGracza.WykonajRuch();
        ruszacz.PrzyjmijRuch(ruchy);

        for (auto& komp : kompiutery)
        {
            komp->czas += czas;
            auto& ruchy = komp->WykonajRuch();
            ruszacz.PrzyjmijRuch(ruchy);
        }

        ruszacz.Ruszaj(czas);
        myszkaGracza.Potworz(event);
        muzykant.GrajEfekty(ruszacz);

        // przyspiesz jesli zostaly same komputery
        if (!przyspieszenie_bez_gracza && misja_ustawienia.JedenGracz() && !rozgrywka.Graczu(nr_gracza).aktywny)
        {
            int mnoznik_czasu = 5;
            for (auto komp : kompiutery)
                komp->szybkosc_komputera *= mnoznik_czasu;
            ruszacz.szybkosc *= mnoznik_czasu;
            przyspieszenie_bez_gracza = true;
        }

        window.clear();

        if (!to_serwer)
        {
            interfejs = interfejs_rozgrywki(interfejs, misja_ustawienia, rozgrywka, wyswietlacz, ruchGracza.WybranyDomek(), myszkaGracza.skupiony);
            GUI::aplikacja().show_bottom_gui(view, interfejs);
            wyswietlacz.WyswietlTlo(window);
        }

        czas_gry = (double)(clock() - start_gry) / CLOCKS_PER_SEC;

        if (!to_serwer)
        {
            ruchGracza.Wyswietlaj(window);
            wyswietlacz.Wyswietlaj(window);
        }

        //ZAKONCZENIE GRY
        if (rozgrywka.liczba_aktywnych_graczy == 1)
        {
            muzykant.Zamilcz();

            if (!to_serwer)
            {
                GUI::aplikacja().finish_viewport_render(view);
                if (interfejs != nullptr)
                    GUI::aplikacja().remove_active_window(interfejs);
            }

            for (auto& g : rozgrywka.gracze)
            {
                if (g.aktywny)
                {
                    misja_ustawienia.ile_kto_wygranych[g.numer]++;
                    if (misja_ustawienia.Zwyciezca() < 0 && !to_serwer)
                        zakonczenie_gry(g, nr_gracza);
                    break;
                }
            }

            auto zwyciezca_meczu = misja_ustawienia.Zwyciezca();
            if (zwyciezca_meczu >= 0 && !to_serwer)
            {
                zakonczenie_meczu(misja_ustawienia, rozgrywka);
            }
            trwa_gra = false;

            break;
        }

        GUI::aplikacja().okno.display();

        //Sleep(16); zaimplementujmy sensowne ograniczenie tutaj? inaczej zawsze jest max 50 fpsów, a gdy faktycznie gra każda klatka trwa długo (mamy 40 fps, to nas to przesuwa do 24)
    }

    if (!to_serwer)
        GUI::aplikacja().finish_viewport_render(view);

    GUI::aplikacja().reset_view();

    if(!to_serwer && interfejs != nullptr)
        GUI::aplikacja().remove_active_window(interfejs);
    return 0;
}

int misja(MisjaUstawienia& misja_ustawienia)
{
    Ruszacz ruszacz;
    return misja(misja_ustawienia, ruszacz);
}