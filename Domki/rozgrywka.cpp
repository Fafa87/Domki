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

void Rozgrywka::ZniszczTwor(Twor* twor) 
{
    if (IsType<Domek>(twor)) {
        Domek* domek = (Domek*)twor;
        auto it = domki.begin();
        for (; it != domki.end(); it++)
        {
            if (&(*it) == domek)
                break;
        }
        if (it != domki.end())
        {
            ZabierzTwor(domek);
            domki.erase(it);
        }
    }
    else if (IsType<Ludek>(twor)) {
        Ludek* armia = (Ludek*)twor;
        auto it = armie.begin();
        for (; it != armie.end(); it++)
        {
            if (&(*it) == armia)
                break;
        }
        if (it != armie.end())
        {
            ZabierzTwor(armia);
            armie.erase(it);
        }
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

void UaktualnijRozmiar(Domek& domek, double nowa)
{
    if (domek.poziom <= 5 && domek.max_liczebnosc != -1)domek.rozmiar = 15 + 3 * domek.poziom + 6 * log(nowa + 1.0) / log(1000);
    else domek.rozmiar = 36;
}

void Rozgrywka::ZmienLiczebnosc(Domek & domek, double nowa)
{
    domek.liczebnosc = nowa;
    UaktualnijRozmiar(domek, nowa);
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

    UaktualnijRozmiar(domek, domek.liczebnosc);

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

void Rozgrywka::DajTwor(const Twor* twor)
{
    twor->gracz->liczba_tworow++;
    if (twor->gracz->liczba_tworow && !twor->gracz->aktywny && twor->gracz->numer)
    {
        liczba_aktywnych_graczy++;
        twor->gracz->aktywny = true;
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
    if (walka_w_polu && ludek.liczebnosc > 0)
    {
        for (auto& armia : armie) if (&armia != &ludek && armia.liczebnosc > 0)
        {
            double odl = Odleglosc(ludek, armia);
            if (odl < (armia.rozmiar + ludek.rozmiar) / 2)
                return &armia;
        }
    }
    return nullptr;
}

Twor * Rozgrywka::Zlokalizuj(int x, int y, int z)
{
    double min_odleglosc;
    Twor* najblizszy = nullptr;
    for (auto& dom : domki)
    {
        PD punkt(x, y);
        PD roz = punkt - dom.polozenie;
        double odleglosc = sqrt(roz.x * roz.x + roz.y * roz.y);
        if (odleglosc < 1.5 * dom.rozmiar + z) {
            if (najblizszy == nullptr || odleglosc < min_odleglosc) {
                najblizszy = &dom;
                min_odleglosc = odleglosc;
            }
        }
    }
	for (auto& armia : armie)
	{
		PD punkt(x, y);
		PD roz = punkt - armia.polozenie;
        double odleglosc = sqrt(roz.x * roz.x + roz.y * roz.y);
		if (odleglosc < 1.5 * armia.rozmiar + z)
            if (najblizszy == nullptr || odleglosc < min_odleglosc) {
                najblizszy = &armia;
                min_odleglosc = odleglosc;
            }
	}
    return najblizszy;
}

void wyznaczProsta(PD punkt1, PD punkt2, double &A, double &B, double &C) {
    if (punkt1.x == punkt2.x && punkt1.y == punkt2.y) return;
    double x_y = punkt1.x - punkt2.x, y_x = punkt1.y - punkt2.y, t, s1, s2;
    if (x_y != 0.0) {
        t = -y_x / x_y;
        s1 = punkt1.x * t + punkt1.y;
        s2 = punkt2.x * t + punkt2.y;
        if (abs(s1 - s2) < 0.000001) {
            B = 1.0;
            C = -s1;
            A = B * t;
        }
    }
    else {
        A = 1.0;
        B = 0.0;
        C = -punkt1.x;
    }
}

bool wyznaczPrzeciecie(double A, double B, double C, double D, double E, double F, PD& punkt, PD middlePoint = {0, 0}) {
    if (B * E == 0.0) {
        if (B == 0.0 && E == 0.0) {
            punkt.x = -C / A;
            if (-F / D != punkt.x) return false;
            else punkt.y = middlePoint.y;
        }
        else if (B == 0.0) {
            punkt.x = -C / A;
            punkt.y = (-F - D * punkt.x) / E;
        }
        else if (E == 0.0) {
            punkt.x = -F / D;
            punkt.y = (-C - A * punkt.x) / B;
        }
    }
    else {
        double t = D / E - A / B, u = C / B - F / E;
        if (t == 0.0) {
            if (u == 0.0)
                punkt.x = middlePoint.x;
            else return false;
        }
        else punkt.x = u / t;
        punkt.y = -(C / B + A / B * punkt.x);
    }
}

bool Rozgrywka::PolaczDomki(Domek& domek1, Domek& domek2) {
    if (CzyMoznaPolaczycDomki(domek1, domek2)) {
        domek1.drogi.push_back(&domek2);
        domek2.drogi.push_back(&domek1);
        return true;
    }
    return false;
}

bool Rozgrywka::CzyTamJestDroga(int x, int y, double odleglosc) {
    for(auto &domek1 : domki)
        for (auto& domek2 : domek1.drogi) {
            double A, B, C, D, E, F;
            PD punktX;
            wyznaczProsta(domek1.polozenie, domek2->polozenie, A, B, C);

            if (B == 0.0) {
                D = 0.0;
                E = 1.0;
                F = -(double)y;
            }
            else if(A == 0.0){
                D = 1.0;
                E = 0.0;
                F = -(double)x;
            }
            else {
                D = - B / A;
                E = 1.0;
                F = -(D * (double)x + E * (double)y);
            }

            wyznaczPrzeciecie(A, B, C, D, E, F, punktX);

            double odleglosc_od_drogi = sqrt((punktX.x - (double)x) * (punktX.x - (double)x) + (punktX.y - (double)y) * (punktX.y - (double)y));
            
            if (odleglosc_od_drogi <= odleglosc) return true;
       }
    return false;
}

bool Rozgrywka::CzyMoznaPolaczycDomki(Domek& domek1, Domek& domek2) {
    if (&domek1 == &domek2) return false;
    for (auto domek : domek1.drogi) {
        if (domek == &domek2) return false;
    }
    PD pointA = domek1.polozenie, pointB = domek2.polozenie, pointX;
    double A, B, C, D, E, F;
    if (pointA.x == pointB.y && pointA.y == pointB.y) return false;
    wyznaczProsta(pointA, pointB, A, B, C);

    for (auto& domek3 : domki) {
        for(auto& domek4: domek3.drogi)
            if (domek3.uid < domek4->uid) {
                PD pointC = domek3.polozenie, pointD = domek4->polozenie;

                wyznaczProsta(pointC, pointD, D, E, F);

                if (!wyznaczPrzeciecie(A, B, C, D, E, F, pointX, {min(max(pointA.y, pointB.y), max(pointC.y, pointD.y)), min(max(pointA.y, pointB.y), max(pointC.y, pointD.y))})) continue;

                if ((pointX == pointA || pointX == pointB) && (pointX == pointC || pointX == pointD)) continue;
                if (pointX.x >= min(pointA.x, pointB.x) - domek1.rozmiar && pointX.y >= min(pointA.y, pointB.y) - domek1.rozmiar &&
                    pointX.x <= max(pointA.x, pointB.x) + domek1.rozmiar && pointX.y <= max(pointA.y, pointB.y) + domek1.rozmiar &&
                    pointX.x >= min(pointC.x, pointD.x) - domek1.rozmiar && pointX.y >= min(pointC.y, pointD.y) - domek1.rozmiar &&
                    pointX.x <= max(pointC.x, pointD.x) + domek1.rozmiar && pointX.y <= max(pointC.y, pointD.y) + domek1.rozmiar) return false;
                
               }
    }

    return true;
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

Domek Rozgrywka::stworz_domyslny_domek() {
        Domek nowy;
        nowy.max_liczebnosc = 100;
        nowy.gracz = &this->Graczu(0);
        nowy.produkcja = 1.0;
        nowy.typdomku = TypDomku::kMiasto;
        ZmienLiczebnosc(nowy, 50);
        return nowy;
}
