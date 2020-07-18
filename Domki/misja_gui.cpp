#include "misja.h"
#include "gui.h"

#include "ext_string.h"


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
            info_zapelnienie_etykieta->SetText(L"Zape³nienie:");
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
            info_poziom_etykieta->SetText(L"Szybkoœæ:");
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

    // dorzuc nag³owek ma³y
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

    table->SetClass("Ranking");

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
            GUI::aplikacja().add_property_to_batch("Button#" + graczId, "BackgroundColor", gracz.kolor);
            //GUI::aplikacja().pulpit.SetProperty<sf::Color>("Button#" + graczId, "BackgroundColor", gracz.kolor);
            //GUI::aplikacja().pulpit.SetProperty("Button#" + graczId, "FontSize", 28);           
            //GUI::aplikacja().pulpit.SetProperty("Label#" + graczId, "FontSize", 28);
            //GUI::aplikacja().pulpit.SetProperty("Label#" + graczId, "FontName", "Grafika/BuxtonSketch.ttf");
            GUI::aplikacja().add_property_to_batch("Label#" + graczProdukcjaId, "Color", gracz.kolor);
            //GUI::aplikacja().pulpit.SetProperty("Label#" + graczProdukcjaId, "Color", gracz.kolor);
            //GUI::aplikacja().pulpit.SetProperty("Label#" + graczProdukcjaId, "FontSize", 28);
            GUI::aplikacja().add_property_to_batch("Label#" + graczModernizacjaId, "Color", sf::Color::Yellow);
            //GUI::aplikacja().pulpit.SetProperty("Label#" + graczModernizacjaId, "Color", sf::Color::Yellow);
            //GUI::aplikacja().pulpit.SetProperty("Label#" + graczModernizacjaId, "FontSize", 28);
            GUI::aplikacja().add_property_to_batch("Label#" + graczSzybkoscId, "Color", sf::Color::Black);
            //GUI::aplikacja().pulpit.SetProperty("Label#" + graczSzybkoscId, "Color", sf::Color::Black);
            //GUI::aplikacja().pulpit.SetProperty("Label#" + graczSzybkoscId, "FontSize", 28);
            GUI::aplikacja().add_property_to_batch("Label#" + graczLudkiId, "FontName", "Grafika/waltographUI.ttf");
            //GUI::aplikacja().pulpit.SetProperty("Label#" + graczLudkiId, "FontName", "Grafika/waltographUI.ttf");
            //GUI::aplikacja().pulpit.SetProperty("Label#" + graczLudkiId, "FontSize", 28);
            if (stan.Zwyciezca() == nr)
            {
                GUI::aplikacja().add_property_to_batch("Button#" + graczId, "FontSize", 80.f);
                GUI::aplikacja().add_property_to_batch("Label#" + graczId, "FontSize", 80.f);

                //GUI::aplikacja().pulpit.SetProperty("Button#" + graczId, "FontSize", 80.f);
                //GUI::aplikacja().pulpit.SetProperty("Label#" + graczId, "FontSize", 80.f);
            }

            auto wartosc = sfg::Button::Create(to_string(stan.ile_kto_wygranych[nr]));
            wartosc->SetId(graczId);
            wartosc->SetClass("Wartosci");
            auto nazwa = sfg::Label::Create(gracz.nazwa);
            nazwa->SetAlignment(sf::Vector2f(0.5, 0.5));
            nazwa->SetId(graczId);
            nazwa->SetClass("Wartosci");

            auto sila_gracza = rozgrywka.SilaGracza(nr);
            auto ludki = sfg::Label::Create(to_string(std::get<0>(sila_gracza)));
            ludki->SetAlignment(sf::Vector2f(0.5, 0.5));
            ludki->SetId(graczLudkiId);
            ludki->SetClass("Wartosci_Ludki");
            auto produkcja = sfg::Label::Create(to_string(std::get<1>(sila_gracza)));
            produkcja->SetAlignment(sf::Vector2f(0.5, 0.5));
            produkcja->SetId(graczProdukcjaId);
            produkcja->SetClass("Wartosci");
            auto modernizacja = sfg::Label::Create(to_string(std::get<2>(sila_gracza)));
            modernizacja->SetAlignment(sf::Vector2f(0.5, 0.5));
            modernizacja->SetId(graczModernizacjaId);
            modernizacja->SetClass("Wartosci");
            auto szybkosc = sfg::Label::Create(to_string(std::get<3>(sila_gracza)));
            szybkosc->SetAlignment(sf::Vector2f(0.5, 0.5));
            szybkosc->SetId(graczSzybkoscId);
            szybkosc->SetClass("Wartosci");

            table->Attach(wartosc, sf::Rect<sf::Uint32>(0, i, 1, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL);// , sf::Vector2f(10.f, 10.f));
            table->Attach(nazwa, sf::Rect<sf::Uint32>(1, i, 1, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL);//, sf::Vector2f(10.f, 10.f));
            table->Attach(ludki, sf::Rect<sf::Uint32>(2, i, 1, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL);//, sf::Vector2f(10.f, 10.f));
            table->Attach(produkcja, sf::Rect<sf::Uint32>(3, i, 1, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL);//, sf::Vector2f(10.f, 10.f));
            table->Attach(modernizacja, sf::Rect<sf::Uint32>(4, i, 1, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL);//, sf::Vector2f(10.f, 10.f));
            table->Attach(szybkosc, sf::Rect<sf::Uint32>(5, i, 1, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL);//, sf::Vector2f(10.f, 10.f));
            i++;
        }
    }

    GUI::aplikacja().commit_properties_batch();

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