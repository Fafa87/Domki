#include "rozgrywka.h"

int Twor::last_uid = 0;

void Rozgrywka::PrzerwijGre()
{
    liczba_aktywnych_graczy = 1;
}

void Rozgrywka::PoddajGracza(Gracz & gracz)
{
    for (auto& twor : this->armie)
        if (gracz.numer == twor.gracz->numer)
            twor.gracz = &this->Graczu(0);

    for (auto& twor : this->domki)
        if (gracz.numer == twor.gracz->numer)
            twor.gracz = &this->Graczu(0);

    gracz.liczba_tworow = 0;
    gracz.aktywny = 0;
    liczba_aktywnych_graczy--;
}

Domek & Rozgrywka::Domku(int numer)
{
    auto it = domki.begin();
    std::advance(it, numer);
    return *it;
}

Gracz & Rozgrywka::Graczu(int numer)
{
    auto it = gracze.begin();
    std::advance(it, numer);
    return *it;
}

Gracz & Rozgrywka::Graczu(string nazwa)
{
    return *std::find_if(gracze.begin(), gracze.end(), [nazwa](Gracz& g) { return g.nazwa == nazwa; });
}

std::tuple<int,int,int,int> Rozgrywka::SilaGracza(int nr_gracza)
{
    int ludki=0, produkcja=0, modernizacja=0,szybkosc=0;
    auto tearmie = armie.begin();
    auto tedomki = domki.begin();
    for (; tearmie != armie.end(); tearmie++)if (tearmie->gracz->numer == nr_gracza)ludki += tearmie->liczebnosc;
    for (; tedomki != domki.end(); tedomki++)if (tedomki->gracz->numer == nr_gracza)
    {
        if (tedomki->typdomku == TypDomku::kMiasto)
            produkcja += tedomki->poziom;
        else if (tedomki->typdomku == TypDomku::kZbrojownia)modernizacja += tedomki->poziom;
        else if (tedomki->typdomku == TypDomku::kStajnia)szybkosc += tedomki->poziom;
        ludki += tedomki->liczebnosc;
    }
    return make_tuple(ludki, produkcja, modernizacja,szybkosc);
}

int Rozgrywka::OcenaGracza(int nr_gracza) {
    int suma = 0;
    Gracz &gracz = Graczu(nr_gracza);
     for (auto &domek : domki)
           if (domek.gracz == &gracz) {
               suma += domek.liczebnosc;
               for (int wspolczynnik = 25, poziom = 1; poziom <= domek.poziom; suma += wspolczynnik, wspolczynnik *= 2, poziom++);
           }
    return suma;
}

void Rozgrywka::ZniszczLudka(Ludek* ludek)
{
    auto it = armie.begin();
    for (; it != armie.end(); it++)
    {
        if (&(*it) == ludek)
            break;
    }

    if (it != armie.end())
    {
        ZabierzTwor(ludek);
        armie.erase(it);
    }
}

bool Rozgrywka::Zyje(Ludek * ludek)
{
    auto it = armie.begin();
    for (; it != armie.end(); it++)
    {
        if (&(*it) == ludek)
            return true;
    }
    return false;
}

void Rozgrywka::ZmienLiczebnosc(Domek & domek, double nowa)
{
    domek.liczebnosc = nowa;
    if (domek.poziom <= 5&&domek.max_liczebnosc!=-1)domek.rozmiar = 15 + 3 * domek.poziom +6 * log(nowa + 1.0) / log(1000);
    else domek.rozmiar = 36;
}

void Rozgrywka::ZmienLiczebnosc(Ludek & ludek, double nowa)
{
    ludek.liczebnosc = nowa;
    ludek.rozmiar = 10 + 10 * log(nowa+1.0) / log(1000);
}

void Rozgrywka::ZmienPoziom(Domek & domek, int nowy_poziom)
{
    if (nowy_poziom > 0)
    {
        domek.max_liczebnosc = 100 * (1 << (nowy_poziom - 1));
    }
    else
    {
        domek.typdomku = TypDomku::kPole;
        domek.max_liczebnosc = 0;
    }
    domek.poziom = nowy_poziom;
    if(domek.max_liczebnosc != -1)domek.rozmiar = 15 + 3 * domek.poziom + 6 * log(domek.liczebnosc + 1.0) / log(1000);
    else domek.rozmiar = domek.poziom * domek.poziom;
}

void Rozgrywka::TracLudki(Ludek & ludek, double ile)
{
    ludek.tarcza = std::max(0,ludek.tarcza - (int)ile);
    ile -= (double)ludek.tarcza;
    if (ile > 0)ludek.liczebnosc = std::max(0.0,ludek.liczebnosc-ile);
}

void Rozgrywka::ZabierzTwor(const Twor* twor)
{
    twor->gracz->liczba_tworow--;
    if (twor->gracz->liczba_tworow == 0)
    {
        if (twor->gracz->aktywny) liczba_aktywnych_graczy--;
        twor->gracz->aktywny = false;
    }
}

double Rozgrywka::Odleglosc(const Twor& twor1, const Twor& twor2)
{
    PD polozenie_1 = twor1.polozenie;
    PD polozenie_2 = twor2.polozenie;

    PD w = (polozenie_1 - polozenie_2);
    return sqrt(w.x * w.x + w.y * w.y);
}

Ludek * Rozgrywka::Spotkanie(Ludek & ludek)
{
    if (walka_w_polu)
    {
        for (auto& armia : armie) if (&armia != &ludek)
        {
            double odl = Odleglosc(ludek, armia);
            if (odl < (armia.rozmiar + ludek.rozmiar) / 2)
                return &armia;
        }
    }
    return nullptr;
}

Twor * Rozgrywka::Zlokalizuj(int x, int y)
{
    for (auto& dom : domki)
    {
        PD punkt(x, y);
        PD roz = punkt - dom.polozenie;
        if (sqrt(roz.x * roz.x + roz.y * roz.y) < 1.5 * dom.rozmiar)
            return &dom;
    }
	for (auto& armia : armie)
	{
		PD punkt(x, y);
		PD roz = punkt - armia.polozenie;
		if (sqrt(roz.x * roz.x + roz.y * roz.y) < 1.5 * armia.rozmiar)
			return &armia;
	}
    return nullptr;
}

double Rozgrywka::PoliczAtakDomku(const Domek & domek, int liczba)
{
    if (liczba == -1)
        liczba = domek.liczebnosc;
    double wspolczynnik_walki = 0.1;// 10poz kuzni na tarcze ludka
    auto sila = SilaGracza(domek.gracz->numer);
    auto sila_ludkow = std::get<2>(sila);
    return (int)((double)sila_ludkow*wspolczynnik_walki*(double)liczba) + liczba;
}

double Rozgrywka::PoliczObroneDomku(const Domek & domek)
{
    if (domek.typdomku == TypDomku::kFort)
        return domek.liczebnosc * (domek.poziom + 1.0);
    else if (domek.typdomku == TypDomku::kWieza)
        return domek.liczebnosc * 6.0 / (6.0 - domek.poziom);
    return domek.liczebnosc;
}

double Rozgrywka::PoliczSzybkoscDomku(const Domek & domek)
{
    double wspolczynnik_ruchu = 0.5;// 2poz stajni na szybkosc
    auto sila = SilaGracza(domek.gracz->numer);
    auto szybkosc_ludkow = std::get<3>(sila);
    return szybkosc_ludkow * wspolczynnik_ruchu + 1.0;
}

Domek* Rozgrywka::WskaznikDomek(int uid)
{
    for (auto &dom : domki)
    {
        if (dom.uid == uid)
        {
            return &dom;
        }
    }
    return nullptr;
}

Ludek* Rozgrywka::WskaznikLudek(int uid)
{
    for (auto &ludek : armie)
    {
        if (ludek.uid == uid)
        {
            return &ludek;
        }
    }
    return nullptr;
}

int Rozgrywka::nr_zwyciezcy(bool same_komputery) {
    if (same_komputery) { // SPRAWDZAM CZY SA SAME KOMPUTERY I JESLI TAK, TO WYBIERAM ZWYCIEZCE NA PODSTAWIE LICZBY LUDKOW
        int zwyciezca = -1, max_suma = 0;
        for (auto &gracz : gracze) 
            if(gracz.aktywny){
                int suma = OcenaGracza(gracz.numer);
                if (suma > max_suma) {
                    max_suma = suma;
                    zwyciezca = gracz.numer;
                }
        }
        return zwyciezca;
    }
    else if (liczba_aktywnych_graczy == 1) {
        for (auto &gracz : gracze)
            if (gracz.aktywny) return gracz.numer;
    }
    else if (cel_gry.nazwa_celu == "Zbieranie") {
        int nr = -1;
        for (auto &gracz : gracze) {
            if (gracz.aktywny) {
                int ludki = std::get<0>(SilaGracza(gracz.numer));
                if (ludki >= cel_gry.wymagany_zbior && (nr == -1 || ludki > std::get<0>(SilaGracza(nr)))) 
                    nr = gracz.numer;
            }
        }
        return nr;
    }
    else if (cel_gry.nazwa_celu == "KOTH" && Domku(cel_gry.do_zdobycia - 1).gracz->numer != 0) {
        return Domku(cel_gry.do_zdobycia - 1).gracz->numer;
    }
    return -1;
}
