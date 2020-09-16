#include "gui_okna.h"
#include "misja.h"


WyborMisjiKontrolki::WyborMisjiKontrolki()
{
}

shared_ptr<WyborMisjiKontrolki> WyborMisjiKontrolki::DlaMisji(bool opcja_ile_ludzi, bool opcja_oszustwa, bool opcja_walka_w_polu, bool opcja_punkty_kontrolne)
{
    auto kontrolki = make_shared<WyborMisjiKontrolki>();
    kontrolki->opcja_misja = true;
    kontrolki->opcja_oszustwa = opcja_oszustwa;
    kontrolki->opcja_walka_w_polu = opcja_walka_w_polu;
    kontrolki->opcja_punkty_kontrolne = opcja_punkty_kontrolne;
    kontrolki->opcja_ile_ludzi = opcja_ile_ludzi;
    return kontrolki;
}

shared_ptr<WyborMisjiKontrolki> WyborMisjiKontrolki::DlaKampanii()
{
    auto kontrolki = make_shared<WyborMisjiKontrolki>();
    kontrolki->opcja_kampania = true;
    kontrolki->opcja_oszustwa = false;
    kontrolki->opcja_walka_w_polu = false;
    kontrolki->opcja_punkty_kontrolne = false;
    kontrolki->opcja_ile_ludzi = false;
    return kontrolki;
}

void WyborMisjiKontrolki::DodajZestaw(shared_ptr<sfg::Box> box)
{
    auto ile_ludzi_etykieta = sfg::Label::Create("Ile ludzi: ");
    auto ile_ludzi_wartosc = sfg::Label::Create("1");
    this->ile_ludzi_pasek = sfg::Scale::Create(1, 6, 1);
    ile_ludzi_pasek->SetValue(0);
    ile_ludzi_pasek->SetIncrements(1, 2);
    ile_ludzi_pasek->GetAdjustment()->GetSignal(sfg::Adjustment::OnChange).Connect(
        [ile_ludzi_wartosc, this] {
        ile_ludzi_wartosc->SetText(to_string((int)this->ile_ludzi_pasek->GetValue()));
    });

    auto wybor_etykieta = sfg::Label::Create("");
    auto separator = sfg::Separator::Create();
    if (opcja_misja)
    {
        wybor_etykieta->SetText("Misja: ");
        this->misja_grupa = sfg::ComboBox::Create();
        for (auto l : wczytaj_liste_folderow("Plansza"))
            this->misja_grupa->AppendItem(l);
        this->misja_grupa->SelectItem(2);

        this->misja_nazwa = sfg::ComboBox::Create();
        for (auto l : wczytaj_liste_plansz("Plansza\\" + this->misja_grupa->GetSelectedText()))
            this->misja_nazwa->AppendItem(l);
        this->misja_nazwa->SelectItem(2);

        this->misja_grupa->GetSignal(sfg::ComboBox::OnSelect).Connect([this]
        {
            this->misja_nazwa->Clear();
            for (auto l : wczytaj_liste_plansz("Plansza\\" + this->misja_grupa->GetSelectedText()))
                this->misja_nazwa->AppendItem(l);
            this->misja_nazwa->SelectItem(0);
            auto misja_wybrana = "Plansza\\" + this->misja_grupa->GetSelectedText() + "\\" + this->misja_nazwa->GetSelectedText();
            auto misja_ustawienia = wczytaj_meta(misja_wybrana);
            auto max_ludzi = misja_ustawienia.komputery.size() + 1;
            ile_ludzi_pasek->SetRange(0, max_ludzi);
            ile_ludzi_pasek->SetValue(max_ludzi);
        });

        this->misja_nazwa->GetSignal(sfg::ComboBox::OnSelect).Connect(
            [this] {
            auto misja_wybrana = "Plansza\\" + this->misja_grupa->GetSelectedText() + "\\" + this->misja_nazwa->GetSelectedText();
            auto misja_ustawienia = wczytaj_meta(misja_wybrana);
            auto max_ludzi = misja_ustawienia.komputery.size() + 1;
            ile_ludzi_pasek->SetRange(0, max_ludzi);
            ile_ludzi_pasek->SetValue(max_ludzi);
        });
        this->misja_nazwa->SelectItem(2);
        this->misja_nazwa->GetSignal(sfg::ComboBox::OnSelect)();
    }

    if (opcja_kampania);
    {
        wybor_etykieta->SetText("Kampanie: ");
        this->misja_nazwa = sfg::ComboBox::Create();
        for (auto l : wczytaj_liste_folderow("Kampanie"))
            this->misja_nazwa->AppendItem(l);
        this->misja_nazwa->SelectItem(0);
    }

    auto szybkosc_etykieta = sfg::Label::Create(L"Szybkoœæ: ");
    this->szybkosc_pasek = sfg::Scale::Create(0.3, 4, 0.1);
    this->szybkosc_pasek->SetValue(1.5);

    auto do_ilu_etykieta = sfg::Label::Create("Do ilu: ");
    auto do_ilu_wartosc = sfg::Label::Create("1");
    this->do_ilu_pasek = sfg::Scale::Create(1, 6, 1);
    this->do_ilu_pasek->SetValue(1);
    this->do_ilu_pasek->SetIncrements(1, 2);
    this->do_ilu_pasek->GetAdjustment()->GetSignal(sfg::Adjustment::OnChange).Connect(
        [do_ilu_wartosc, this] {
        do_ilu_wartosc->SetText(to_string((int)this->do_ilu_pasek->GetValue()));
    });

    auto trudnosc_etykieta = sfg::Label::Create("Trudnosc: ");
    auto trudnosc_wartosc = sfg::Label::Create("5");
    this->trudnosc_pasek = sfg::Scale::Create(1, 10, 1);
    this->trudnosc_pasek->SetValue(5);
    this->trudnosc_pasek->SetIncrements(1, 3);
    this->trudnosc_pasek->GetAdjustment()->GetSignal(sfg::Adjustment::OnChange).Connect(
        [trudnosc_wartosc, this] {
        trudnosc_wartosc->SetText(to_string((int)trudnosc_pasek->GetValue()));
    });

    auto inne_etykieta = sfg::Label::Create("Inne: ");
    this->oszustwa_ptaszek = sfg::CheckButton::Create("Oszustwa: ");
    this->oszustwa_ptaszek->SetActive(false);
    this->walka_w_polu_ptaszek = sfg::CheckButton::Create("Walka w polu: ");
    this->walka_w_polu_ptaszek->SetActive(true);
    this->punkty_kontrolne_ptaszek = sfg::CheckButton::Create("Punkty kontrolne: ");
    this->punkty_kontrolne_ptaszek->SetActive(true);

    auto tabelka = sfg::Table::Create();
    tabelka->SetRowSpacings(10);

    int row_top = 0;
    tabelka->Attach(separator, sf::Rect<sf::Uint32>(0, row_top++, 4, 1));
    tabelka->Attach(wybor_etykieta, sf::Rect<sf::Uint32>(0, row_top, 1, 1));
    if (opcja_misja)
    {
        tabelka->Attach(misja_grupa, sf::Rect<sf::Uint32>(1, row_top++, 2, 1));
        tabelka->Attach(misja_nazwa, sf::Rect<sf::Uint32>(1, row_top++, 2, 1));
    }
    if (opcja_kampania)
    {
        tabelka->Attach(misja_nazwa, sf::Rect<sf::Uint32>(1, row_top++, 2, 1));
    }
    tabelka->Attach(szybkosc_etykieta, sf::Rect<sf::Uint32>(0, row_top, 1, 1));
    tabelka->Attach(szybkosc_pasek, sf::Rect<sf::Uint32>(1, row_top++, 2, 1));
    tabelka->Attach(trudnosc_etykieta, sf::Rect<sf::Uint32>(0, row_top, 1, 1));
    tabelka->Attach(trudnosc_pasek, sf::Rect<sf::Uint32>(1, row_top, 1, 1));
    tabelka->Attach(trudnosc_wartosc, sf::Rect<sf::Uint32>(2, row_top++, 1, 1));
    tabelka->Attach(do_ilu_etykieta, sf::Rect<sf::Uint32>(0, row_top, 1, 1));
    tabelka->Attach(do_ilu_pasek, sf::Rect<sf::Uint32>(1, row_top, 1, 1));
    tabelka->Attach(do_ilu_wartosc, sf::Rect<sf::Uint32>(2, row_top++, 1, 1));
    if (opcja_ile_ludzi)
    {
        tabelka->Attach(ile_ludzi_etykieta, sf::Rect<sf::Uint32>(0, row_top, 1, 1));
        tabelka->Attach(ile_ludzi_pasek, sf::Rect<sf::Uint32>(1, row_top, 1, 1));
        tabelka->Attach(ile_ludzi_wartosc, sf::Rect<sf::Uint32>(2, row_top++, 1, 1));
    }
    if (opcja_oszustwa)
    {
        tabelka->Attach(oszustwa_ptaszek, sf::Rect<sf::Uint32>(1, row_top++, 2, 1));
    }
    if (opcja_walka_w_polu)
    {
        tabelka->Attach(walka_w_polu_ptaszek, sf::Rect<sf::Uint32>(1, row_top++, 2, 1));
    }
    if (opcja_punkty_kontrolne)
    {
        tabelka->Attach(punkty_kontrolne_ptaszek, sf::Rect<sf::Uint32>(1, row_top++, 2, 1));
    }
    box->Pack(tabelka, false, false);
}

string WyborMisjiKontrolki::MisjaGrupa()
{
    return this->misja_grupa->GetSelectedText();
}

string WyborMisjiKontrolki::MisjaNazwa()
{
    return this->misja_nazwa->GetSelectedText();
}

string WyborMisjiKontrolki::KampaniaNazwa()
{
    return this->misja_nazwa->GetSelectedText();
}

int WyborMisjiKontrolki::IleLudzi()
{
    if (opcja_ile_ludzi == false)
        return 1;
    return (int)this->ile_ludzi_pasek->GetValue();
}

double WyborMisjiKontrolki::Szybkosc()
{
    return this->szybkosc_pasek->GetValue();
}

int WyborMisjiKontrolki::DoIluWygranych()
{
    return (int)this->do_ilu_pasek->GetValue();
}

int WyborMisjiKontrolki::PoziomTrudnosci()
{
    return (int)this->trudnosc_pasek->GetValue();
}

bool WyborMisjiKontrolki::Oszustwa()
{
    if (opcja_oszustwa == false)
        return false;
    return this->oszustwa_ptaszek->IsActive();
}

bool WyborMisjiKontrolki::WalkaWPolu()
{
    if (opcja_walka_w_polu == false)
        return true;
    return this->walka_w_polu_ptaszek->IsActive();
}

bool WyborMisjiKontrolki::PunktyKontrolne()
{
    if (opcja_punkty_kontrolne == false)
        return true;
    return this->punkty_kontrolne_ptaszek->IsActive();
}
