#include "razem.h"

#include "misja.h"
#include "gui_okna.h"
#include "../MultiSerwer/mastery.h"

std::shared_ptr<sfg::Window> serwer_menu_tutaj(std::shared_ptr<sfg::Window> glowne, sf::Music & muzyka, string nazwa)
{
    return start_serwer_menu(glowne, muzyka, nazwa, 
        [](string komenda_startu)
    {
        GUI::aplikacja().zalozone_gry.push_back(start_nowej_gry_dla_wielu(komenda_startu));
        LOG(INFO) << "Zalozone gry: " << GUI::aplikacja().zalozone_gry.size();
    });
}

std::shared_ptr<sfg::Window> serwer_menu_planeta(std::shared_ptr<sfg::Window> glowne, sf::Music & muzyka, mastery::Klient* master_klient)
{
    return start_serwer_menu(glowne, muzyka, master_klient->gracz.nazwa,
        [master_klient](string komenda_startu)
    {
        // TMP to walnie na pewno :)
        auto tokeny_start = split(komenda_startu, ' ');
        tokeny_start.erase(tokeny_start.begin());
        tokeny_start.erase(tokeny_start.end()-1);
        auto serce_komendy = join(tokeny_start, " ");
        master_klient->komendy.add("/START: " + serce_komendy);
        LOG(INFO) << "Prosba o zalozenie gry: " << serce_komendy;
    });
}

std::shared_ptr<sfg::Window> start_serwer_menu(std::shared_ptr<sfg::Window> glowne, sf::Music& muzyka, string nazwa,
    function<void(string)> zakladaj_fun)
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
        true, // ile ludzi
        false, false, false);
    kontrolki->DodajZestaw(box);
    
    auto zakladaj = sfg::Button::Create(L"Zak³adaj!");
    zakladaj->GetSignal(sfg::Widget::OnLeftClick).Connect(
        [&muzyka, okno, kontrolki, zakladaj_fun] {
        zakladaj_fun(start_generuj_komende_startu(kontrolki->MisjaGrupa(), kontrolki->MisjaNazwa(),
            kontrolki->DoIluWygranych(), kontrolki->Szybkosc(), kontrolki->IleLudzi(), PORT_TCP)); // TODO uzyj informacji o trudnosci);
        GUI::aplikacja().pop_active_window(okno);
    });

    auto powrot = sfg::Button::Create(L"Powrót");
    powrot->GetSignal(sfg::Widget::OnLeftClick).Connect(
        [okno] {
        GUI::aplikacja().pop_active_window(okno);
    });
    
    box->Pack(sfg::Separator::Create(), false, false);
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
    auto size_planeta = sf::Vector2f(max(19 * size.x / 20.0, 800.0), max(19 * size.y / 20.0, 600.0));
    okno->SetRequisition(size_planeta);

    // GÓRNY PANEL
    auto dane_master_serwera = sfg::Label::Create("Dane podpietego master serwera");
    dane_master_serwera->SetText(master_klient->adres_serwer.ToString());


    auto gracz_etykieta = sfg::Label::Create("Gracz: " + master_klient->gracz.nazwa);
    auto pokoj_etykieta = sfg::Label::Create(L"Aktualny pokój:");
    auto pokoj = sfg::Entry::Create("hol");
    pokoj->SetRequisition(sf::Vector2f(200, 0));

    auto prawe_skrzydlo = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
    prawe_skrzydlo->Pack(gracz_etykieta);
    prawe_skrzydlo->SetSpacing(70);
    prawe_skrzydlo->Pack(pokoj_etykieta);
    prawe_skrzydlo->Pack(pokoj);

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
    odpal->GetSignal(sfg::Widget::OnLeftClick).Connect(
        [glowne, &muzyka, master_klient] {
        if (!master_klient->RozgrywkaTrwa())
        {
            auto okno_zaloz = serwer_menu_planeta(glowne, muzyka, master_klient);
            if (okno_zaloz != nullptr)
                GUI::aplikacja().set_active_window(okno_zaloz);
        }
    });
    auto dolacz = sfg::Button::Create(L"Do³¹cz teraz");
    dolacz->GetSignal(sfg::Widget::OnLeftClick).Connect(
        [okno, master_klient, &muzyka] {
        if (master_klient->RozgrywkaTrwa())
        {
            muzyka.stop();
            GUI::aplikacja().hide_all_windows();
            start_klient(muzyka, master_klient->gracz.nazwa, master_klient->rozgrywka_pokoju);
            muzyka.play();
            GUI::aplikacja().set_active_window(okno);
        }
    });
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
            //if (chat->GetText().getSize())
            //    chat->SetText(pisak->GetText() + "\n" + chat->GetText());
            //else
            //    chat->SetText(pisak->GetText());
            pisak->SetText("");
        }
        if (pokoj->HasFocus() && ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Return)
        {
            master_klient->IdzDo(pokoj->GetText());
        }
        return !master_klient->polaczony;
    },
    [odpal, dolacz, chat_z_paskiem, chat, ludki, pokoj, master_klient]() {
        string tekst;
        if (master_klient->odebrane.try_take(tekst) == code_machina::BlockingCollectionStatus::Ok)
        {
            if (chat->GetText().getSize())
                chat->SetText(tekst + "\n" + chat->GetText());
            else
                chat->SetText(tekst);
        }
        ludki->SetText(join(master_klient->KtoJestObok(), "\n"));
        if (master_klient->rozgrywka_pokoju.ip.size())
        {
            if (odpal->GetClass() != "Nieaktywny")
                odpal->SetClass("Nieaktywny");
            if (dolacz->GetClass() != "Aktywny")
                dolacz->SetClass("Aktywny");
        }
        else
        {
            if (dolacz->GetClass() != "Nieaktywny")
                dolacz->SetClass("Nieaktywny");
            if (odpal->GetClass() != "Aktywny")
                odpal->SetClass("Aktywny");
        }
    });
    master_klient->Rozlacz();
    GUI::aplikacja().pop_active_window(okno);

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
        auto okno_zaloz = serwer_menu_tutaj(glowne, muzyka, nazwa_edit->GetText());
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

        auto adres_planety = GUI::aplikacja().ini.Get("multi", "adres_planety", "localhost:60");
        wykonaj_masterklient(KontekstSwiata::o().klient, "polacz " + adres_planety); // TMP
        Sleep(5000); // TMP tutaj powinno byæ jakieœ okienko, ¿e trwa ³¹czenie
        if (KontekstSwiata::o().klient->polaczony)
        {
            planeta_okno(glowne, muzyka, KontekstSwiata::o().klient);
            KontekstSwiata::o().klient->Rozlacz(); // wyczysc masterklienta 
        }
        else
        {
            okno_info(L"Brak kontaktu z planet¹");
            KontekstSwiata::o().klient->Rozlacz();
        }
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