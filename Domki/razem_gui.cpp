#include "razem.h"

#include "misja.h"
#include "../MultiSerwer/mastery.h"

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

    auto szybkosc_etykieta = sfg::Label::Create(L"Szybkoœæ: ");
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

    auto powrot = sfg::Button::Create(L"Powrót");
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



std::shared_ptr<sfg::Window> planeta_okno(std::shared_ptr<sfg::Window> glowne, sf::Music& muzyka, mastery::Klient* master_klient)
{
    auto okno = sfg::Window::Create(sfg::Window::Style::BACKGROUND | sfg::Window::Style::SHADOW);
    auto size = GUI::aplikacja().okno.getSize();
    auto size_planeta = sf::Vector2f(max(size.x / 2.0, 800.0), max(size.y / 2.0, 600.0));
    okno->SetRequisition(size_planeta);

    // GÓRNY PANEL
    auto dane_master_serwera = sfg::Label::Create("Dane podpietego master serwera");
    dane_master_serwera->SetText(master_klient->adres_serwer.ToString());

    auto pokoj = sfg::Entry::Create("hol");
    pokoj->SetRequisition(sf::Vector2f(200, 0));
    auto prawe_skrzydlo = sfg::Alignment::Create();
    prawe_skrzydlo->SetAlignment(sf::Vector2f(1, 0.5));
    prawe_skrzydlo->Add(pokoj);

    auto gorny_panel = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
    gorny_panel->SetSpacing(30);
    gorny_panel->Pack(dane_master_serwera, false, false);
    gorny_panel->Pack(prawe_skrzydlo, false, false);

    // CENTRUM
    auto chat = sfg::Label::Create("");
    chat->SetAlignment(sf::Vector2f(0, 0));
    chat->SetClass("Razem");
    auto chat_z_paskiem = sfg::ScrolledWindow::Create();
    chat_z_paskiem->SetScrollbarPolicy(sfg::ScrolledWindow::VERTICAL_ALWAYS);
    chat_z_paskiem->AddWithViewport(chat);
    chat_z_paskiem->SetRequisition(sf::Vector2f(400.f, 300.f));
    auto ramka_chatu = sfg::Frame::Create("");
    ramka_chatu->Add(chat_z_paskiem);

    auto ludki = sfg::Label::Create("Ludki"); // TMP merge extensions to SFGUI so that we can attach to proper event
    ludki->SetRequisition(sf::Vector2f(200, 0));
    ludki->SetAlignment(sf::Vector2f(0, 0));
    ludki->SetClass("Razem");
    auto ramka_pokoju = sfg::Frame::Create("Ludzie w pokoju");
    ramka_pokoju->Add(ludki);

    auto srodek_panel = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
    srodek_panel->SetSpacing(10);
    srodek_panel->Pack(ramka_chatu, true, true);
    srodek_panel->Pack(ramka_pokoju, false, false);

    auto pisak = sfg::Entry::Create(""); // TMP merge extensions to SFGUI so that we can attach to proper event

    // KLIKANIE
    auto odpal = sfg::Button::Create(L"Postaw serwer");
    auto dolacz = sfg::Button::Create(L"Do³¹cz teraz");
    auto powrot = sfg::Button::Create(L"Powrót");
    powrot->GetSignal(sfg::Widget::OnLeftClick).Connect(
        [okno, master_klient] {
        master_klient->Rozlacz();
        GUI::aplikacja().pop_active_window(okno);
    });

    auto dol_panel = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
    dol_panel->SetSpacing(50);
    dol_panel->Pack(odpal, false, false);
    dol_panel->Pack(dolacz, false, false);
    dol_panel->Pack(powrot, false, false);

    // CA£OŒÆ
    auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
    box->SetSpacing(10);
    box->Pack(gorny_panel, false, false);
    box->Pack(srodek_panel, true, true);
    box->Pack(pisak, false, false);
    box->Pack(sfg::Separator::Create(), false, true);
    box->Pack(dol_panel, true, false);
    okno->Add(box);

    GUI::aplikacja().center_window(okno);
    GUI::aplikacja().set_active_window(okno);

    // ZAPYTAJ SERWER O STAN W POKOJU

    // PÊTELKA
    GUI::aplikacja().process_loop(
    [master_klient, pisak, chat, pokoj](sf::Event ev) {
        if (pisak->HasFocus() && ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Return)
        {
            master_klient->komendy.add(pisak->GetText());
            if (chat->GetText().getSize())
                chat->SetText(chat->GetText() + "\n" + pisak->GetText());
            else
                chat->SetText(pisak->GetText());
            pisak->SetText("");
        }
        if (pokoj->HasFocus() && ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Return)
        {
            master_klient->IdzDo(pokoj->GetText());
        }
        return !master_klient->polaczony;
    },
    [odpal, dolacz, chat, ludki, pokoj, master_klient]() {
        string tekst;
        if (master_klient->odebrane.try_take(tekst) == code_machina::BlockingCollectionStatus::Ok)
        {
            if (chat->GetText().getSize())
                chat->SetText(chat->GetText() + "\n" + tekst);
            else
                chat->SetText(tekst);
        }
        ludki->SetText(join(master_klient->KtoJestObok(), "\n"));
    });

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
    separator->SetRequisition(sf::Vector2f(10, 20));

    auto zaloz = sfg::Button::Create(L"Za³ó¿ tutaj");
    zaloz->GetSignal(sfg::Widget::OnLeftClick).Connect(
        [&muzyka, nazwa_edit, glowne] {
        auto okno_zaloz = start_serwer_menu(glowne, muzyka, nazwa_edit->GetText());
        if (okno_zaloz != nullptr)
            GUI::aplikacja().set_active_window(okno_zaloz);
    });

    auto dolacz = sfg::Button::Create(L"Do³¹cz tutaj");
    dolacz->GetSignal(sfg::Widget::OnLeftClick).Connect(
        [okno, &muzyka, nazwa_edit] {
        muzyka.stop();
        GUI::aplikacja().hide_all_windows();
        start_klient(muzyka, nazwa_edit->GetText());
        muzyka.play();
        GUI::aplikacja().set_active_window(okno);
    });

    auto planeta = sfg::Button::Create(L"Planeta");
    planeta->GetSignal(sfg::Widget::OnLeftClick).Connect(
        [okno, &muzyka, nazwa_edit, glowne] {
        start_masterklient(nazwa_edit->GetText());
        wykonaj_masterklient(KontekstSwiata::o().klient, "polacz localhost:60"); // TMP
        Sleep(2000); // TMP tutaj powinno byæ jakieœ okienko, ¿e trwa ³¹czenie
        planeta_okno(glowne, muzyka, KontekstSwiata::o().klient);
        KontekstSwiata::o().klient->Rozlacz();
        // wyczysc masterklienta 
    });

    auto powrot = sfg::Button::Create(L"Powrót");
    powrot->GetSignal(sfg::Widget::OnLeftClick).Connect(
        [okno] {
        GUI::aplikacja().pop_active_window(okno);
    });

    box->SetSpacing(10);
    box->Pack(tytul, false, false);
    box->Pack(nazwa_etykieta, false, false);
    box->Pack(nazwa_edit, false, false);
    box->Pack(sfg::Separator::Create(), false, true);

    box->Pack(zaloz, false, false);
    box->Pack(dolacz, false, false);
    box->Pack(planeta, false, false);

    box->Pack(sfg::Separator::Create(), false, true);
    box->Pack(powrot, true, false);
    okno->Add(box);

    GUI::aplikacja().stretch_up_down(okno);

    return okno;
}