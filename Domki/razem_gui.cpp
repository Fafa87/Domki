#include "razem.h"

#include "misja.h"

std::shared_ptr<sfg::Window> start_serwer_menu(std::shared_ptr<sfg::Window> glowne, sf::Music& muzyka, string nazwa)
{
    auto okno = sfg::Window::Create(sfg::Window::Style::BACKGROUND | sfg::Window::Style::SHADOW);

    okno->SetRequisition(sf::Vector2f(glowne->GetAllocation().width, 0));
    okno->SetAllocation(glowne->GetAllocation());

    auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
    auto tytul = sfg::Label::Create(WERSJA);
    tytul->SetId("Naglowek");

    auto ile_ludzi_etykieta = sfg::Label::Create("Ile ludzi: ");
    auto ile_ludzi_wartosc = sfg::Label::Create("1");
    auto ile_ludzi_pasek = sfg::Scale::Create(1, 6, 1);
    ile_ludzi_pasek->SetValue(0);
    ile_ludzi_pasek->SetIncrements(1, 2);
    ile_ludzi_pasek->GetAdjustment()->GetSignal(sfg::Adjustment::OnChange).Connect(
        [ile_ludzi_wartosc, ile_ludzi_pasek] {
        ile_ludzi_wartosc->SetText(to_string((int)ile_ludzi_pasek->GetValue()));
    });

    auto wybor_etykieta = sfg::Label::Create("Misja: ");
    auto separator = sfg::Label::Create("");
    auto wybor_lista_foldery = sfg::ComboBox::Create();
    for (auto l : wczytaj_liste_folderow("Plansza"))
        wybor_lista_foldery->AppendItem(l);
    wybor_lista_foldery->SelectItem(2);

    auto wybor_lista = sfg::ComboBox::Create();
    for (auto l : wczytaj_liste_plansz("Plansza\\" + wybor_lista_foldery->GetSelectedText()))
        wybor_lista->AppendItem(l);
    wybor_lista->SelectItem(2);

    wybor_lista_foldery->GetSignal(sfg::ComboBox::OnSelect).Connect([ile_ludzi_pasek, wybor_lista, wybor_lista_foldery]
    {
        wybor_lista->Clear();
        for (auto l : wczytaj_liste_plansz("Plansza\\" + wybor_lista_foldery->GetSelectedText()))
            wybor_lista->AppendItem(l);
        wybor_lista->SelectItem(0);
        auto misja_wybrana = "Plansza\\" + wybor_lista_foldery->GetSelectedText() + "\\" + wybor_lista->GetSelectedText();
        auto misja_ustawienia = wczytaj_meta(misja_wybrana);
        auto max_ludzi = misja_ustawienia.komputery.size() + 1;
        ile_ludzi_pasek->SetRange(0, max_ludzi);
        ile_ludzi_pasek->SetValue(max_ludzi);
    });

    wybor_lista->GetSignal(sfg::ComboBox::OnSelect).Connect(
        [ile_ludzi_pasek, wybor_lista, wybor_lista_foldery] {
        auto misja_wybrana = "Plansza\\" + wybor_lista_foldery->GetSelectedText() + "\\" + wybor_lista->GetSelectedText();
        auto misja_ustawienia = wczytaj_meta(misja_wybrana);
        auto max_ludzi = misja_ustawienia.komputery.size() + 1;
        ile_ludzi_pasek->SetRange(0, max_ludzi);
        ile_ludzi_pasek->SetValue(max_ludzi);
    });
    wybor_lista->SelectItem(2);
    wybor_lista->GetSignal(sfg::ComboBox::OnSelect)();

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

    auto zakladaj = sfg::Button::Create(L"Zak³adaj!");
    zakladaj->GetSignal(sfg::Widget::OnLeftClick).Connect(
        [&muzyka, nazwa, do_ilu_pasek, szybkosc_pasek, wybor_lista, wybor_lista_foldery, ile_ludzi_pasek] {
        GUI::aplikacja().zalozone_gry.push_back(
            start_nowej_gry_dla_wielu(wybor_lista_foldery->GetSelectedText(), wybor_lista->GetSelectedText(), (int)do_ilu_pasek->GetValue(), szybkosc_pasek->GetValue(), ile_ludzi_pasek->GetValue()));
        LOG(INFO) << "Zalozone gry: " << GUI::aplikacja().zalozone_gry.size();
    });

    auto powrot = sfg::Button::Create("Powrot");
    powrot->GetSignal(sfg::Widget::OnLeftClick).Connect(
        [okno] {
        GUI::aplikacja().pop_active_window(okno);
    });

    auto tabelka = sfg::Table::Create();
    tabelka->SetRowSpacings(10);

    tabelka->Attach(separator, sf::Rect<sf::Uint32>(0, 0, 4, 1));
    tabelka->Attach(wybor_etykieta, sf::Rect<sf::Uint32>(0, 1, 1, 1));
    tabelka->Attach(wybor_lista_foldery, sf::Rect<sf::Uint32>(1, 1, 2, 1));
    tabelka->Attach(wybor_lista, sf::Rect<sf::Uint32>(1, 2, 2, 1));
    tabelka->Attach(szybkosc_etykieta, sf::Rect<sf::Uint32>(0, 3, 1, 1));
    tabelka->Attach(szybkosc_pasek, sf::Rect<sf::Uint32>(1, 3, 2, 1));
    tabelka->Attach(do_ilu_etykieta, sf::Rect<sf::Uint32>(0, 4, 1, 1));
    tabelka->Attach(do_ilu_pasek, sf::Rect<sf::Uint32>(1, 4, 1, 1));
    tabelka->Attach(do_ilu_wartosc, sf::Rect<sf::Uint32>(2, 4, 1, 1));
    tabelka->Attach(ile_ludzi_etykieta, sf::Rect<sf::Uint32>(0, 5, 1, 1));
    tabelka->Attach(ile_ludzi_pasek, sf::Rect<sf::Uint32>(1, 5, 1, 1));
    tabelka->Attach(ile_ludzi_wartosc, sf::Rect<sf::Uint32>(2, 5, 1, 1));

    box->SetSpacing(10);
    box->Pack(tytul, false, false);
    box->Pack(tabelka, false, false);
    box->Pack(zakladaj, false, false);
    box->Pack(powrot, false, false);
    okno->Add(box);

    GUI::aplikacja().stretch_up_down(okno);

    return okno;
}

std::shared_ptr<sfg::Window> wielu_graczy_menu(std::shared_ptr<sfg::Window> glowne, sf::Music& muzyka)
{
    auto okno = sfg::Window::Create(sfg::Window::Style::BACKGROUND | sfg::Window::Style::SHADOW);

    //okno->SetRequisition(sf::Vector2f(480, 0));
    okno->SetRequisition(sf::Vector2f(glowne->GetAllocation().width, 0));
    okno->SetAllocation(glowne->GetAllocation());

    auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
    auto tytul = sfg::Label::Create(WERSJA);
    tytul->SetId("Naglowek");

    auto nazwa_etykieta = sfg::Label::Create("Nazwa gracza: ");
    auto nazwa_edit = sfg::Entry::Create();
    auto separator = sfg::Label::Create("");

    auto zaloz = sfg::Button::Create(L"Za³ó¿");
    zaloz->GetSignal(sfg::Widget::OnLeftClick).Connect(
        [&muzyka, nazwa_edit, glowne] {
        auto okno_zaloz = start_serwer_menu(glowne, muzyka, nazwa_edit->GetText());
        if (okno_zaloz != nullptr)
            GUI::aplikacja().set_active_window(okno_zaloz);
    });

    auto dolacz = sfg::Button::Create(L"Do³¹cz");
    dolacz->GetSignal(sfg::Widget::OnLeftClick).Connect(
        [okno, &muzyka, nazwa_edit] {
        muzyka.stop();
        GUI::aplikacja().hide_all_windows();
        start_klient(muzyka, nazwa_edit->GetText());
        muzyka.play();
        GUI::aplikacja().set_active_window(okno);
    });

    auto powrot = sfg::Button::Create("Powrot");
    powrot->GetSignal(sfg::Widget::OnLeftClick).Connect(
        [okno] {
        GUI::aplikacja().pop_active_window(okno);
    });

    box->SetSpacing(10);
    box->Pack(tytul, false, false);
    box->Pack(nazwa_etykieta, false, false);
    box->Pack(nazwa_edit, false, false);
    box->Pack(separator, true, true);

    box->Pack(zaloz, false, false);
    box->Pack(dolacz, false, false);
    box->Pack(powrot, true, false);
    okno->Add(box);

    GUI::aplikacja().stretch_up_down(okno);

    return okno;
}