#include "dzialacze.h"
#include <set>
#include <string>
#include<ctime>
#include<cmath>

MyszDecydent::MyszDecydent(sf::RenderWindow & okno, Rozgrywka & rozgrywka, Gracz & gracz) : okno(okno), rozgrywka(rozgrywka), gracz(gracz)
{
}

void MyszDecydent::Klik(double x, double y)
{
}

void MyszDecydent::Przetworz(sf::Event zdarzenie)
{
    if (zdarzenie.type == sf::Event::MouseButtonPressed)
    {
        //zdarzenie.mouseButton.button == sf::Mouse::Left <---wazna formula
        sf::Vector2i pixelPos = sf::Mouse::getPosition(okno);
        auto polozenie_kliku = okno.mapPixelToCoords(pixelPos);
        Twor* klikniety = rozgrywka.Zlokalizuj(polozenie_kliku.x, polozenie_kliku.y);
        if (klikniety != nullptr && IsType<Domek>(klikniety))
        {
            if (wybrany != (Domek*)klikniety&& klikniety->gracz->numer == gracz.numer&&zdarzenie.mouseButton.button == sf::Mouse::Left)//wybór własnego domku
               wybrany = (Domek*)klikniety;
            else if (wybrany != (Domek*)klikniety&& klikniety->gracz->numer != gracz.numer && zdarzenie.mouseButton.button == sf::Mouse::Middle)//wybór nie wlasnego domku
                wybrany = (Domek*)klikniety;

            if (wybrany != nullptr && wybrany != klikniety && zdarzenie.mouseButton.button == sf::Mouse::Right)//przesyl ludkow tylko prawym klawiszem myszy
            {
                if (cel != klikniety || klikniecia.back().second != zdarzenie.mouseButton.button)
                {
                    klikniecia.clear();
                    cel = (Domek*)klikniety;
                }
            }
            else if (wybrany != nullptr && wybrany == klikniety && zdarzenie.mouseButton.button == sf::Mouse::Left)//ulepszanie
            {
                if (cel != klikniety || klikniecia.back().second != zdarzenie.mouseButton.button)
                {
                    klikniecia.clear();
                    cel = (Domek*)klikniety;
                }
            }
        }
        else
        {
            klikniecia.clear();
            wybrany = nullptr;
            cel = nullptr;	
        }
        klikniecia.push_back(make_pair(clock(), zdarzenie.mouseButton.button));
    }

    if (zdarzenie.type == sf::Event::KeyPressed)
    {
        if (wybrany != nullptr )// + && wybrany->gracz == &gracz
        {
            if (zdarzenie.key.code == sf::Keyboard::Tilde)
                nacisniety = '`';
            else if (zdarzenie.key.code == sf::Keyboard::Num1)
                nacisniety = '1';
            else if (zdarzenie.key.code == sf::Keyboard::Num2)
                nacisniety = '2';
            else if (zdarzenie.key.code == sf::Keyboard::Num3)
                nacisniety = '3';
            else if (zdarzenie.key.code == sf::Keyboard::Num4)
                nacisniety = '4';
            else if (zdarzenie.key.code == sf::Keyboard::Num5)
                nacisniety = '5';
            else if (zdarzenie.key.code == sf::Keyboard::T)
                nacisniety = 'T';
        }
    }
}

vector<Rozkaz*> MyszDecydent::WykonajRuch()
{
    vector<Rozkaz*> res;
  //  if (wybrany != nullptr && wybrany->gracz != &gracz) // to jest pod komentarzem aby moc sterowac kompem - trzeba wczesniej wcisnac srodkowy klawisz
  //  {
       // cel = nullptr;
       // wybrany = nullptr;
       // nacisniety = 0;
 //   }
    
    if (klikniecia.size() > 0 && (clock() - klikniecia.back().first > 0.33 * CLOCKS_PER_SEC || klikniecia.size() >= 4)) // tutaj trzeba dodac else z przodu bo wczesniej jest if w komentarzu
    {
        if (wybrany != nullptr&&cel != nullptr&&cel == wybrany)
        {
            // ulepszanie
            if (klikniecia.size() == 2)
            {
                punkty_kontrolne.erase(wybrany); //punkt kontrolny wylacza sie poprzez ulepszenie

                auto r = new UlepszRozkaz(wybrany);
                res.push_back(r);

                klikniecia.clear();
                cel = nullptr;
                nacisniety = 0;
            }
            else if (klikniecia.size() >= 3)
            {
                klikniecia.clear();

                punkty_kontrolne[wybrany] = wybrany;

                wybrany = nullptr;
                cel = nullptr;
                nacisniety = 0;
            }
        }
        else if (wybrany != nullptr&&cel != nullptr&&cel != wybrany)
        {
            // wysylanie
            if (klikniecia.size() <= 2)
            {
                punkty_kontrolne.erase(wybrany); //punkt kontrolny wylacza sie poprzez wyslanie jednostek

                double frakcja = 1;
                if (klikniecia.size() == 1)
                    frakcja = 0.5;

                // utworz i zwroc rozkaz 
                auto r = new WymarszRozkaz(wybrany, cel);
                r->ulamek = frakcja;
                res.push_back(r);

                if (klikniecia.size() == 2)wybrany = nullptr; // odznaczaj tylko gdy zostala wyslana calosc
                //wybrany = nullptr; 
                cel = nullptr;
                nacisniety = 0;
                klikniecia.clear();
            }
            else if (klikniecia.size() >= 3)
            {
                punkty_kontrolne[wybrany] = cel;
                //wybrany->punkt_kontrolny = cel;

                auto r = new WymarszRozkaz(wybrany, cel);
                r->ulamek = 1;
                res.push_back(r);

                wybrany = nullptr;
                cel = nullptr;
                nacisniety = 0;
                klikniecia.clear();
            }

        }
        klikniecia.clear();
        cel = nullptr;
    }

    // przetwarzaj punkty kontrolne
    for (auto pk_iter = punkty_kontrolne.begin(); pk_iter != punkty_kontrolne.end();)
    {
        auto pk = *pk_iter;
        if ( (pk.first == pk.second && pk.first->poziom >= 5) || (pk.first != pk.second && pk.first->typdomku != TypDomku::kMiasto) )//pk.first->gracz != &gracz ||
            pk_iter = punkty_kontrolne.erase(pk_iter);
        else {
            if (pk.first == pk.second)
            {
                auto x = new UlepszRozkaz(pk.first);
                res.push_back(x);
            }
            else if (pk.first->liczebnosc * 10 >= pk.first->max_liczebnosc||pk.first->liczebnosc >= 100.0)
            {
                auto x = new WymarszRozkaz(pk.first, pk.second);
                x->ulamek = 1;
                res.push_back(x);
            }
            pk_iter++;
        }
    }

/*	if (klikniecia.size() > 0 && clock() - klikniecia.back() >= 3.0 * CLOCKS_PER_SEC) // co trzy sekundy braku akcji domek jest odznaczany
    {        // do zrobienia -> zmniejszanie się okręgu wokół domku symbolizujące malejący czas
        wybrany = nullptr;
        cel = nullptr;
        nacisniety = 0;
        klikniecia.clear();
    }*/ //narazie bez tego

    if (nacisniety != 0 && wybrany != nullptr)
    {
        switch (nacisniety)
        {
        case '`':
            res.push_back(new BurzRozkaz(wybrany));
            break;
        case '1':
            res.push_back(new PrzebudujRozkaz(wybrany, TypDomku::kMiasto));
            break;
        case '2':
            res.push_back(new PrzebudujRozkaz(wybrany, TypDomku::kFort));
            break;
        case '3':
            res.push_back(new PrzebudujRozkaz(wybrany, TypDomku::kZbrojownia));
            break;
        case '4':
            res.push_back(new PrzebudujRozkaz(wybrany, TypDomku::kWieza));
            break;
        case '5':
            res.push_back(new PrzebudujRozkaz(wybrany, TypDomku::kStajnia));
            break;
        case 'T':
            res.push_back(new Testpower(wybrany));
            break;
        }
        //wybrany = nullptr; //to umozliwia szybkie burzenie lub/oraz przebudowę 
        cel = nullptr;
        nacisniety = 0;
    }
    return res;
}


Ruszacz::Ruszacz()
{
}

void Ruszacz::Ruszaj(double czas)
{
    ile_armii_idzie = armie_ktore_dotarly = 0;

    WykonajRuchy();
    PrzesuwajLudkow(czas);
    WalczLudkami(czas);
    Strzelaj(czas);
    Produkuj(czas);
}

void Ruszacz::PrzyjmijRuch(vector<Rozkaz*> rozkazy)
{
    kolejka_do_wykonania.insert(kolejka_do_wykonania.end(), rozkazy.begin(), rozkazy.end());
}

void Ruszacz::WykonajRuchy()
{
    for (auto r : kolejka_do_wykonania)
    {
        if (IsType<WymarszRozkaz>(r))
        {
            auto& ruch = *(WymarszRozkaz*)r;

            auto liczba = int(ruch.skad->liczebnosc * ruch.ulamek);
            if (liczba > 0 && &ruch.skad != &ruch.dokad)
            {
                if (ruch.skad->drogi.size() == 0 || count(ruch.skad->drogi.begin(), ruch.skad->drogi.end(), ruch.dokad))
                {
                    rozgrywka->ZmienLiczebnosc(*ruch.skad, ruch.skad->liczebnosc - liczba);

                    rozgrywka->armie.push_back(Ludek(*ruch.dokad));
                    Ludek& nowaArmia = rozgrywka->armie.back();
                    nowaArmia.gracz = ruch.skad->gracz;
                    nowaArmia.polozenie = ruch.skad->polozenie;
                    nowaArmia.wyglad = Wyglad::kWojownik;
                    rozgrywka->ZmienLiczebnosc(nowaArmia, liczba);

                    //ZMIERZ SILE LUDKOW I NADAJ IM SZYBKOSC
                    nowaArmia.tarcza = rozgrywka->PoliczAtakDomku(*ruch.skad, liczba) - liczba;
                    nowaArmia.szybkosc_ludka = rozgrywka->PoliczSzybkoscDomku(*ruch.skad);

                    ruch.skad->gracz->liczba_tworow++;
                }
            }
        }
        else if (IsType<UlepszRozkaz>(r))
        {
            auto ulepsz = (UlepszRozkaz*)r;

            if (ulepsz->kogo->ulepszanie == true&&ulepsz->kogo->poziom>0&&ulepsz->kogo->liczebnosc - ulepsz->kogo->max_liczebnosc / 2.0 > 0&&ulepsz->kogo->poziom<=4&&ulepsz->kogo->poziom>0)
                {
                rozgrywka->ZmienLiczebnosc(*ulepsz->kogo, ulepsz->kogo->liczebnosc - ulepsz->kogo->max_liczebnosc / 2.0);
                ulepsz->kogo->poziom++;
                ulepsz->kogo->max_liczebnosc = 2 * ulepsz->kogo->max_liczebnosc;
                }
        }
        else if (IsType<BurzRozkaz>(r))
        {
            auto ulepsz = (BurzRozkaz*)r;

            if (ulepsz->kogo->ulepszanie == true && ulepsz->kogo->poziom>0&&ulepsz->kogo->liczebnosc > 25.0)
            {
                ulepsz->kogo->poziom--;
                if(ulepsz->kogo->poziom>0)ulepsz->kogo->max_liczebnosc = ulepsz->kogo->max_liczebnosc / 2.0;
                else
                    {
                    ulepsz->kogo->typdomku = TypDomku::kPole;
                    ulepsz->kogo->max_liczebnosc = 0;
                    }
                rozgrywka->ZmienLiczebnosc(*ulepsz->kogo, ulepsz->kogo->liczebnosc - 25.0);
            }
        }
        else if (IsType<PrzebudujRozkaz>(r))
        {
            auto przebuduj = (PrzebudujRozkaz*)r;
            if (przebuduj->kogo->przebudowa==true&&przebuduj->kogo->typdomku!=przebuduj->naco&&przebuduj->kogo->liczebnosc * 2.0 >= przebuduj->kogo->max_liczebnosc&&przebuduj->kogo->poziom>0)
            {
                przebuduj->kogo->typdomku = przebuduj->naco;
                rozgrywka->ZmienLiczebnosc(*przebuduj->kogo, przebuduj->kogo->liczebnosc- przebuduj->kogo->max_liczebnosc/2.0);
            }
            else if (przebuduj->kogo->przebudowa == true && przebuduj->kogo->typdomku != przebuduj->naco&&przebuduj->kogo->liczebnosc >= 25.0&&przebuduj->kogo->poziom == 0)
            {
                przebuduj->kogo->typdomku = przebuduj->naco;
                przebuduj->kogo->poziom = 1;
                przebuduj->kogo->max_liczebnosc = 100.0;
                rozgrywka->ZmienLiczebnosc(*przebuduj->kogo, przebuduj->kogo->liczebnosc - 25.0);
            }
            // TODO ustaw odpowiednio wartości jeśli ulepszenie jest możliwe
        }
        else if (IsType<Testpower>(r))
        {
            auto testpoweruj = (Testpower*)r;
            if (testpoweruj->kogo->poziom == 0)testpoweruj->kogo->typdomku = TypDomku::kMiasto;
            testpoweruj->kogo->poziom = 5;
            testpoweruj->kogo->max_liczebnosc = 1600.0;
            rozgrywka->ZmienLiczebnosc(*testpoweruj->kogo, 1600.0);
        }
    }
    kolejka_do_wykonania.clear();
}

void Ruszacz::PrzesuwajLudkow(double czas)
{
    double przesuniecie = szybkosc * szybkosc_ruchu;
    ile_armii_idzie = rozgrywka->armie.size();
    for (Ludek& armia : rozgrywka->armie)
    {
        if (rozgrywka->Spotkanie(armia) == NULL)
        {
            PD polozenie_cel = armia.cel->polozenie;
            PD polozenie_teraz = armia.polozenie;

            PD wektor_do_celu = (polozenie_cel - polozenie_teraz);
            double dlugosc = sqrt(wektor_do_celu.x * wektor_do_celu.x + wektor_do_celu.y * wektor_do_celu.y);
            if (dlugosc < 0.00001)
                continue;

            PD jednostkowy = wektor_do_celu / dlugosc;
            jednostkowy *= przesuniecie*armia.szybkosc_ludka*czas;

            armia.polozenie += jednostkowy;
        }
    }
}

void Ruszacz::WalczLudkami(double czas)
{
    vector<Ludek*> do_usuniecia;
    for (auto it = rozgrywka->armie.begin(); it != rozgrywka->armie.end(); it++)
    {
        Ludek& armia = *it;
        double odleglosc = rozgrywka->Odleglosc(armia, *armia.cel);
        auto spotkanie = rozgrywka->Spotkanie(armia);
        if (spotkanie != NULL)
        {
            rozgrywka->TracLudki(armia, std::max(5.0+ czas * szybkosc,0.3 * (*spotkanie).liczebnosc*czas * szybkosc));
            rozgrywka->TracLudki(*spotkanie, std::max(5.0+ czas * szybkosc, 0.3 * armia.liczebnosc*czas * szybkosc));

            if (armia.liczebnosc <= 0)
            {
                do_usuniecia.push_back(&(*it));
            }

            if (spotkanie->liczebnosc <= 0)
            {
                do_usuniecia.push_back(spotkanie);
            }
        }
        if (odleglosc < min(armia.cel->rozmiar,armia.rozmiar))
        {
            if (IsType<Domek>(armia.cel))
            {
                Domek* cel = (Domek*)armia.cel;
                if (armia.gracz == armia.cel->gracz)
                {
                    armie_ktore_dotarly++;
                    rozgrywka->ZmienLiczebnosc(*cel, armia.liczebnosc + cel->liczebnosc);
                }
                else
                {
                    double nowa_liczebnosc;
                    if (cel->typdomku == TypDomku::kFort)
                        {
                        cel->liczebnosc = std::max(0.0, cel->liczebnosc - (double)armia.tarcza/((double)cel->poziom+1.0));
                        if (cel->liczebnosc < armia.liczebnosc / (double)((double)cel->poziom + 1.0))
                        {
                            armia.liczebnosc -= (double)((double)cel->poziom + 1.0)*cel->liczebnosc;
                            nowa_liczebnosc = -armia.liczebnosc;
                        }
                        else nowa_liczebnosc = cel->liczebnosc - armia.liczebnosc/(double)((double)cel->poziom + 1.0);
                        }
                    else
                        {
                        cel->liczebnosc = std::max(0.0, cel->liczebnosc - (double)armia.tarcza);
                        nowa_liczebnosc = cel->liczebnosc - armia.liczebnosc;
                        }
                    if (nowa_liczebnosc < 0)
                    {
                        rozgrywka->ZabierzTwor(cel);
                        cel->gracz = armia.gracz;
                        armia.gracz->liczba_tworow++;		
                        armie_ktore_dotarly++;

						cel->poziom = std::min(5,cel->poziom);
                    }
                    rozgrywka->ZmienLiczebnosc(*cel, std::abs(nowa_liczebnosc));
                }

                rozgrywka->ZmienLiczebnosc(armia, std::abs(0));
                do_usuniecia.push_back(&(*it));
            }
        }
        
    }

    for (auto usunieta : do_usuniecia)
    {
        rozgrywka->ZniszczLudka(usunieta);
    }
}

void Ruszacz::Produkuj(double czas)
{
    for (Domek& domek : rozgrywka->domki)
    {
        if (domek.gracz->aktywny&&domek.liczebnosc == domek.max_liczebnosc);
        else if(domek.gracz->aktywny&&domek.liczebnosc<domek.max_liczebnosc&&domek.typdomku == TypDomku::kMiasto)rozgrywka->ZmienLiczebnosc(domek, domek.liczebnosc + szybkosc*czas*domek.produkcja*domek.poziom);
        else if(domek.liczebnosc>domek.max_liczebnosc)rozgrywka->ZmienLiczebnosc(domek,max(domek.liczebnosc - szybkosc*czas*(domek.liczebnosc-domek.max_liczebnosc)/10.0,(double)domek.max_liczebnosc));
    }
}

void Ruszacz::Strzelaj(double czas)
{
    int sila_strzalu = 10.0;
    //tego na razie nie ma
    vector<Ludek*> do_usuniecia;
    for (Ludek& ludek : rozgrywka->armie)
        {
        Domek * domek_cel = ((Domek*)ludek.cel);
        if (domek_cel->typdomku==TypDomku::kWieza&&domek_cel->gracz->numer!=ludek.gracz->numer&&rozgrywka->Odleglosc(ludek,*domek_cel) < 100.0)
            {
            rozgrywka->TracLudki(ludek, sila_strzalu*(double)domek_cel->poziom*(double)domek_cel->poziom*czas* szybkosc);
            if(ludek.liczebnosc==0.0)do_usuniecia.push_back(&ludek);
            }
        }
    for (auto usunieta : do_usuniecia)
    {
        rozgrywka->ZniszczLudka(usunieta);
    }
}

WymarszRozkaz::WymarszRozkaz(Domek * skad, Domek * dokad) : skad(skad), dokad(dokad)
{
}

UlepszRozkaz::UlepszRozkaz(Domek * kogo) : kogo(kogo)
{
}

BurzRozkaz::BurzRozkaz(Domek * kogo) : kogo(kogo)
{
}

PrzebudujRozkaz::PrzebudujRozkaz(Domek * kogo, TypDomku naco) : kogo(kogo), naco(naco)
{
}

Testpower::Testpower(Domek * kogo) : kogo(kogo)
{
}
