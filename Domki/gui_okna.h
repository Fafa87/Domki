#pragma once

#include "gui.h"


class WyborMisjiKontrolki
{
private:
    shared_ptr<sfg::ComboBox> misja_grupa;
    shared_ptr<sfg::ComboBox> misja_nazwa;
    shared_ptr<sfg::Scale> ile_ludzi_pasek;
    shared_ptr<sfg::Scale> szybkosc_pasek;
    shared_ptr<sfg::Scale> do_ilu_pasek;
    shared_ptr<sfg::Scale> trudnosc_pasek;
    bool opcja_ile_ludzi = false;

    bool opcja_misja = false;
    bool opcja_kampania = false;

    shared_ptr<sfg::CheckButton> oszustwa_ptaszek;
    shared_ptr<sfg::CheckButton> walka_w_polu_ptaszek;
    shared_ptr<sfg::CheckButton> punkty_kontrolne_ptaszek;
    bool opcja_oszustwa = false;
    bool opcja_walka_w_polu = false;
    bool opcja_punkty_kontrolne = false;
    bool opcja_do_ilu = true;

public:
    WyborMisjiKontrolki();
    static shared_ptr<WyborMisjiKontrolki> DlaMisji(bool opcja_ile_ludzi = false, bool opcja_oszustwa = false, bool opcja_walka_w_polu = false, bool opcja_punkty_kontrolne = false);
    static shared_ptr<WyborMisjiKontrolki> DlaKampanii();

    void DodajZestaw(shared_ptr<sfg::Box> box);
    
    string KampaniaNazwa();
    string MisjaGrupa();
    string MisjaNazwa();
    int IleLudzi();
    double Szybkosc();
    int DoIluWygranych();

    int PoziomTrudnosci();
    bool Oszustwa();
    bool WalkaWPolu();
    bool PunktyKontrolne();
};

void okno_info(wstring tekst);