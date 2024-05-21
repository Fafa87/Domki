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
        sf::Vector2i pixelPos = sf::Mouse::getPosition(okno);
        auto polozenie_kliku = okno.mapPixelToCoords(pixelPos);
        Twor* klikniety = rozgrywka.Zlokalizuj(polozenie_kliku.x, polozenie_kliku.y);
        if (klikniety != nullptr && IsType<Domek>(klikniety))
        {
            if (zdarzenie.mouseButton.button == sf::Mouse::Left) {
                if (wybrany != (Domek*)klikniety) {
                    wybrany = (Domek*)klikniety;
                    cel = nullptr;
                    klikniecia.clear();
                }
                else if(cel != (Domek*)klikniety)cel = (Domek*)klikniety;
            }
            else if (zdarzenie.mouseButton.button == sf::Mouse::Right) {
                if (wybrany == (Domek*)klikniety)wybrany = nullptr;
                if (kontrola == false && kontrolowany != (Domek*)klikniety && ((Domek*)klikniety) -> kontrola ) {
                    kontrolowany = (Domek*)klikniety;
                    kontrola = true;
                }
                if (cel != (Domek*)klikniety) cel = (Domek*)klikniety;
            }

            //////////////////////////
            if (klikniecia.size() > 0 && zdarzenie.mouseButton.button != klikniecia.back().second)klikniecia.clear();

            klikniecia.push_back(make_pair(clock(), zdarzenie.mouseButton.button));
        }
        else if (klikniety != nullptr && IsType<Ludek>(klikniety) && zdarzenie.mouseButton.button == sf::Mouse::Right) {
            cofany = (Ludek*)klikniety;
        }
        else
        {
            klikniecia.clear();
            wybrany = nullptr;
            cel = nullptr;	
        }  
    }
    else if (kontrola && zdarzenie.type == sf::Event::MouseButtonReleased && zdarzenie.mouseButton.button == sf::Mouse::Right) {
        double x = okno.mapPixelToCoords(sf::Mouse::getPosition(okno)).x - kontrolowany->polozenie.x, y = okno.mapPixelToCoords(sf::Mouse::getPosition(okno)).y - kontrolowany->polozenie.y;
        if (sqrt(x*x + y * y) > kontrolowany->rozmiar * 1.5) {
            wybrany = nullptr;
            cel = nullptr;
            klikniecia.clear();
        }
        kontrola = false;
        kontrolowany = nullptr;
    }
    else if (zdarzenie.type == sf::Event::MouseMoved)
    {
        Skupienie();
        if (kontrola) {
            if (kontrolowany != (Domek*)skupiony&&(wybrany != nullptr || cel != nullptr)) {
                wybrany = nullptr;
                cel = nullptr;
                klikniecia.clear();
            }
            for (auto somsiad : kontrolowany->drogi) {
                double x = okno.mapPixelToCoords(sf::Mouse::getPosition(okno)).x - kontrolowany->polozenie.x, y = okno.mapPixelToCoords(sf::Mouse::getPosition(okno)).y - kontrolowany->polozenie.y ,
                    xx = somsiad->polozenie.x - kontrolowany->polozenie.x, yy = somsiad->polozenie.y - kontrolowany->polozenie.y;
                if (sqrt(x*x + y * y) > sqrt(xx*xx + yy * yy)) {
                    double skala = sqrt(xx*xx + yy * yy) / sqrt(x*x + y * y);
                    x *= skala;
                    y *= skala;
                }
                double odleglosc_wektora = sqrt((x - xx)*(x - xx) + (y - yy)*(y - yy));
                if (somsiad->rozmiar >= odleglosc_wektora) {
                    if (punkty_kontrolne.find(somsiad) != punkty_kontrolne.end() && punkty_kontrolne[somsiad] == kontrolowany) {
                        punkty_kontrolne.erase(somsiad);
                    }
                    punkty_kontrolne[kontrolowany] = somsiad;
                    kontrolowany->szybki_wymarsz = true;
                    kontrolowany = somsiad;
                    break;
                }
            }
        }
        else if (kontrola && skupiony != nullptr && IsType<Domek>(skupiony) && kontrolowany != (Domek*)skupiony&& kontrolowany != nullptr) {
            if (rozgrywka.punkty_kontrolne) {
                for (auto domek : kontrolowany->drogi) {
                    if (domek == skupiony) {
                        if (punkty_kontrolne.find((Domek*)skupiony) != punkty_kontrolne.end() && punkty_kontrolne[(Domek*)skupiony] == kontrolowany) {
                            punkty_kontrolne.erase((Domek*)skupiony);
                        }
                        punkty_kontrolne[kontrolowany] = (Domek*)skupiony;
                        kontrolowany->szybki_wymarsz = true;
                        kontrolowany = (Domek*)skupiony;
                        cel = nullptr;
                        wybrany = nullptr;
                        break;
                    }
                }
            }
        }
        
    }
    else if (zdarzenie.type == sf::Event::KeyPressed)
    {
        if (wybrany != nullptr )
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
        else if (zdarzenie.key.code == sf::Keyboard::Add)nacisniety = '+';
        else if (zdarzenie.key.code == sf::Keyboard::Subtract)nacisniety = '-';

        if (zdarzenie.key.code == sf::Keyboard::Up) nacisniety = 'W';
        else if (zdarzenie.key.code == sf::Keyboard::Down) nacisniety = 'S';
        else if (zdarzenie.key.code == sf::Keyboard::Left) nacisniety = 'A';
        else if (zdarzenie.key.code == sf::Keyboard::Right) nacisniety = 'D';
    }
    else if (zdarzenie.type == sf::Event::MouseWheelMoved) {
        nacisniety = 'X';
        tykiKolaMyszy = zdarzenie.mouseWheel.delta;
    }
}

void MyszDecydent::Skupienie() {

       sf::Vector2i pixelPos = sf::Mouse::getPosition(okno);
       auto polozenie_kursora = okno.mapPixelToCoords(pixelPos);
       skupiony = rozgrywka.Zlokalizuj(polozenie_kursora.x, polozenie_kursora.y);
}

vector<Rozkaz*> MyszDecydent::WykonajRuch()
{
    vector<Rozkaz*> res;
    //PRZESUWANIE MAPY KURSOREM
    sf::Vector2i pixelPos = sf::Mouse::getPosition(okno);
    auto polozenie_kursora = okno.mapPixelToCoords(pixelPos);

    sf::Vector2f min_rog = widok.getCenter() - widok.getSize() / 2.f, max_rog = widok.getCenter() + widok.getSize() / 2.f, dziesiata_czesc = { (float)(widok.getSize().x / 10.0), (float)(widok.getSize().y / 10.0) };

    static clock_t clocker = clock();

    double x_time = ((double)(clock() - clocker)) / CLOCKS_PER_SEC;

    if (x_time >= 0.2) {
        if (abs(polozenie_kursora.y - min_rog.y) < dziesiata_czesc.y) res.push_back(new ZmianaWidoku('W', gracz, widok, bazowy, polozenie_kursora));
        if (abs(polozenie_kursora.y - max_rog.y) < dziesiata_czesc.y) res.push_back(new ZmianaWidoku('S', gracz, widok, bazowy, polozenie_kursora));
        if (abs(polozenie_kursora.x - min_rog.x) < dziesiata_czesc.x) res.push_back(new ZmianaWidoku('A', gracz, widok, bazowy, polozenie_kursora));
        if (abs(polozenie_kursora.x - max_rog.x) < dziesiata_czesc.x) res.push_back(new ZmianaWidoku('D', gracz, widok, bazowy, polozenie_kursora));
        clocker = clock();
    }
    //
    if (rozgrywka.punkty_kontrolne&&wybrany != nullptr) {
        for (auto punkt = punkty_kontrolne.begin(); punkt != punkty_kontrolne.end();punkt++) {
            if (wybrany == (*punkt).first) {
                punkty_kontrolne.erase(punkt);
                break;
            }
        }
    }
    //COFANIE LUDKA
    if (cofany != nullptr) {
        auto r = new CofajLudka(cofany, gracz);
        res.push_back(r);
        cofany = nullptr;
    }
    ///ULEPSZANIE
    else if (wybrany != nullptr && cel != nullptr && klikniecia.size() > 0 && (clock() - klikniecia.back().first > 0.33 * CLOCKS_PER_SEC || klikniecia.size() >= 2))
    {
        if (cel == wybrany)
        {
            if (klikniecia.size() == 1)
            {
                klikniecia.clear();
                cel = nullptr;
                nacisniety = 0;
            }
            else if (klikniecia.size() >= 2&& klikniecia[klikniecia.size() - 1].first - klikniecia[klikniecia.size() - 2].first <= 0.33 * CLOCKS_PER_SEC && clock() - klikniecia.back().first < 0.05 * CLOCKS_PER_SEC) {
                auto r = new UlepszRozkaz(wybrany, gracz);
                res.push_back(r);
            }
            else if (klikniecia.size() >= 2&& clock() - klikniecia.back().first > 0.33 * CLOCKS_PER_SEC)
            {
                klikniecia.clear();
                wybrany = nullptr;
                cel = nullptr;
                nacisniety = 0;
            }
            else if (klikniecia.size() >= 3 && klikniecia[klikniecia.size()-1].first - klikniecia[klikniecia.size() - 2].first <= 0.33 * CLOCKS_PER_SEC && klikniecia[klikniecia.size() - 2].first - klikniecia[klikniecia.size() - 3].first <= 0.33 * CLOCKS_PER_SEC)
            {
                klikniecia.clear();

                if (rozgrywka.punkty_kontrolne)punkty_kontrolne[wybrany] = wybrany;

                wybrany = nullptr;
                cel = nullptr;
                nacisniety = 0;
            }
        }
        // WYMARSZE
        else if (cel != wybrany&&!kontrola)
        {
            if (klikniecia.size() == 1 && clock() - klikniecia.back().first > 0.33 * CLOCKS_PER_SEC && clock() - klikniecia.back().first < 1.0 * CLOCKS_PER_SEC)
            {
                auto r = new WymarszRozkaz(wybrany, cel,gracz);
                r->ulamek = 0.5;
                res.push_back(r);

                cel = nullptr;
                nacisniety = 0;
                klikniecia.clear();
            }
            else if (klikniecia.size() >= 2 && clock() - klikniecia.back().first < 1.0 * CLOCKS_PER_SEC)
            {
                auto r = new WymarszRozkaz(wybrany, cel,gracz);
                r->ulamek = 1.0;
                res.push_back(r);
                for (auto somsiad : wybrany->drogi)if (somsiad == cel) {
                    wybrany = nullptr;
                    break;
                }
                cel = nullptr;
                nacisniety = 0;
                klikniecia.clear();
            }
            else {
                cel = nullptr;
                nacisniety = 0;
                klikniecia.clear();
            }
        }
    }
    
    else if (!kontrola&&wybrany == nullptr && cel != nullptr && cel -> kontrola && klikniecia.size() > 0 && (clock() - klikniecia.back().first > 0.33 * CLOCKS_PER_SEC||klikniecia.size()>=3)) {
        if (!rozgrywka.punkty_kontrolne) {
            klikniecia.clear();
            cel = nullptr;
            nacisniety = 0;
        }
        else if (klikniecia.size() == 1&& clock() - klikniecia.back().first > 0.33 * CLOCKS_PER_SEC)
        {
            //USUN PUNKTY KONTROLNE DO TEGO DOMKU

            for (auto somsiad : cel->drogi) if (punkty_kontrolne.find(somsiad) != punkty_kontrolne.end() && punkty_kontrolne[somsiad] == cel) {
                punkty_kontrolne.erase(somsiad);
            }
            klikniecia.clear();
            cel = nullptr;
            nacisniety = 0;
        }
        else if (klikniecia.size() == 2 && clock() - klikniecia.back().first > 0.33 * CLOCKS_PER_SEC) {
            //USTAW PUNKTY KONTROLNE Z SOJUSZNICYCH POZA NIM SAMYM
            for (auto somsiad : cel->drogi) if(somsiad->gracz->numer == gracz.numer){
                if (punkty_kontrolne.find(somsiad) != punkty_kontrolne.end() && punkty_kontrolne[somsiad] != cel&&(punkty_kontrolne.find(cel) == punkty_kontrolne.end() || punkty_kontrolne[cel] != somsiad)) {
                    punkty_kontrolne.erase(somsiad);
                    punkty_kontrolne[somsiad] = cel;
                }
                if (punkty_kontrolne.find(somsiad) == punkty_kontrolne.end()&&(punkty_kontrolne.find(cel)==punkty_kontrolne.end()||punkty_kontrolne[cel]!=somsiad))punkty_kontrolne[somsiad] = cel;
                somsiad->szybki_wymarsz = true;
            }
            klikniecia.clear();
            cel = nullptr;
            nacisniety = 0;
        }
        else if (klikniecia.size() >= 3)
        {
            //USTAW POZOSTALE PUNKTY KONTROLNE
            if (punkty_kontrolne.find(cel) != punkty_kontrolne.end()) {
                punkty_kontrolne.erase(cel);
            }
            for (auto somsiad : cel->drogi) {
                if (punkty_kontrolne.find(somsiad) != punkty_kontrolne.end() && punkty_kontrolne[somsiad] != cel) {
                    punkty_kontrolne.erase(somsiad);
                }
                if (punkty_kontrolne.find(somsiad) == punkty_kontrolne.end())punkty_kontrolne[somsiad] = cel;
                somsiad->szybki_wymarsz = true;
            }
            klikniecia.clear();
            cel = nullptr;
            nacisniety = 0;
        }
    }
    

    if (rozgrywka.punkty_kontrolne) {
        for (auto pk_iter = punkty_kontrolne.begin(); pk_iter != punkty_kontrolne.end();)
        {
            auto pk = *pk_iter;
            if (pk.first == pk.second&&pk.first->poziom<5)
            {
                auto x = new UlepszRozkaz(pk.first, gracz);
                res.push_back(x);
            }
            else if (pk.first->liczebnosc * 10 >= pk.first->max_liczebnosc || pk.first->liczebnosc >= 100.0 || pk.first->szybki_wymarsz)
            {
                auto x = new WymarszRozkaz(pk.first, pk.second, gracz);
                x->ulamek = 1;
                res.push_back(x);
                pk.first->szybki_wymarsz = false;
            }
            if (pk.first == pk.second&&pk.first->poziom >= 5) {
                auto usun = pk;
                pk_iter++;
                punkty_kontrolne.erase(usun.first);
            }
            else pk_iter++;
        }
    }


    if (nacisniety != 0 && wybrany != nullptr)
    {
        switch (nacisniety)
        {
        case '`':
            res.push_back(new BurzRozkaz(wybrany,gracz));
            break;
        case '1':
            res.push_back(new PrzebudujRozkaz(wybrany, TypDomku::kMiasto, gracz));
            break;
        case '2':
            res.push_back(new PrzebudujRozkaz(wybrany, TypDomku::kFort, gracz));
            break;
        case '3':
            res.push_back(new PrzebudujRozkaz(wybrany, TypDomku::kZbrojownia, gracz));
            break;
        case '4':
            res.push_back(new PrzebudujRozkaz(wybrany, TypDomku::kWieza, gracz));
            break;
        case '5':
            res.push_back(new PrzebudujRozkaz(wybrany, TypDomku::kStajnia, gracz));
            break;
        case 'T':
            res.push_back(new Testpower(wybrany));
            break;
        }
        cel = nullptr;
        nacisniety = 0;
    }
    else if (nacisniety != 0) {
        if(nacisniety != '+' && nacisniety != '-') res.push_back(new ZmianaWidoku(nacisniety, gracz, widok, bazowy, okno.mapPixelToCoords(sf::Mouse::getPosition(okno)), tykiKolaMyszy));
        else res.push_back(new AktualizujPredkosc(nacisniety, gracz));
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
    Strzelaj();
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

            if ((!rozgrywka->oszustwa && ruch.skad->gracz->numer != ruch.kto_wydal_rozkaz->numer) || ruch.skad->gracz->numer == 0)continue;

            auto liczba = int(ruch.skad->liczebnosc * ruch.ulamek);
            if (liczba > 0 && &ruch.skad != &ruch.dokad)
            {
                if (ruch.skad->drogi.size() == 0 || count(ruch.skad->drogi.begin(), ruch.skad->drogi.end(), ruch.dokad))
                {
                    rozgrywka->ZmienLiczebnosc(*ruch.skad, ruch.skad->liczebnosc - liczba);

                    rozgrywka->armie.push_back(Ludek(*ruch.dokad,*ruch.skad));
                    Ludek& nowaArmia = rozgrywka->armie.back();
                    nowaArmia.gracz = ruch.skad->gracz;
                    nowaArmia.polozenie = ruch.skad->polozenie;
                    nowaArmia.wyglad = Wyglad::kWojownik;
                    rozgrywka->ZmienLiczebnosc(nowaArmia, liczba);

                    //ZMIERZ SILE LUDKOW I NADAJ IM SZYBKOSC
                    nowaArmia.tarcza = rozgrywka->PoliczAtakDomku(*ruch.skad, liczba) - liczba;
                    nowaArmia.szybkosc_ludka = rozgrywka->PoliczSzybkoscDomku(*ruch.skad);

                    nowaArmia.droga = rozgrywka->Odleglosc(*ruch.skad, *ruch.dokad);
                    nowaArmia.dystans = 0.0;

                    ruch.skad->gracz->liczba_tworow++;
                }
            }
        }
        else if (IsType<UlepszRozkaz>(r))
        {
            auto ulepsz = (UlepszRozkaz*)r;

            if (!rozgrywka->oszustwa && ulepsz->kogo->gracz->numer != ulepsz->kto_wydal_rozkaz->numer)continue;

            auto koszt = ulepsz->kogo->max_liczebnosc / 2.0;
            if (ulepsz->kogo->ulepszanie == true&&ulepsz->kogo->poziom>0&&ulepsz->kogo->liczebnosc - koszt > 0&&ulepsz->kogo->poziom<=4&&ulepsz->kogo->poziom>0)
                {
                rozgrywka->ZmienPoziom(*ulepsz->kogo, ulepsz->kogo->poziom + 1);
                rozgrywka->ZmienLiczebnosc(*ulepsz->kogo, ulepsz->kogo->liczebnosc - koszt);
                }
        }
        else if (IsType<BurzRozkaz>(r))
        {
            auto burz = (BurzRozkaz*)r;

            if (!rozgrywka->oszustwa && burz->kogo->gracz->numer != burz->kto_wydal_rozkaz->numer)continue;

            if (burz->kogo->ulepszanie == true && burz->kogo->poziom>0&&burz->kogo->liczebnosc > 25.0)
            {
                rozgrywka->ZmienPoziom(*burz->kogo, burz->kogo->poziom - 1);
                rozgrywka->ZmienLiczebnosc(*burz->kogo, burz->kogo->liczebnosc - 25.0);
            }
        }
        else if (IsType<PrzebudujRozkaz>(r))
        {
            auto przebuduj = (PrzebudujRozkaz*)r;

            if (!rozgrywka->oszustwa && przebuduj->kogo->gracz->numer != przebuduj->kto_wydal_rozkaz->numer)continue;

            if (przebuduj->kogo->przebudowa==true&&przebuduj->kogo->typdomku!=przebuduj->naco&&przebuduj->kogo->liczebnosc * 2.0 >= przebuduj->kogo->max_liczebnosc&&przebuduj->kogo->poziom>0)
            {
                przebuduj->kogo->typdomku = przebuduj->naco;
                rozgrywka->ZmienLiczebnosc(*przebuduj->kogo, przebuduj->kogo->liczebnosc- przebuduj->kogo->max_liczebnosc/2.0);
            }
            else if (przebuduj->kogo->przebudowa == true && przebuduj->kogo->typdomku != przebuduj->naco&&przebuduj->kogo->liczebnosc >= 25.0&&przebuduj->kogo->poziom == 0)
            {
                przebuduj->kogo->typdomku = przebuduj->naco;
                rozgrywka->ZmienPoziom(*przebuduj->kogo, 1);
                rozgrywka->ZmienLiczebnosc(*przebuduj->kogo, przebuduj->kogo->liczebnosc - 25.0);
            }
            // TODO ustaw odpowiednio wartości jeśli ulepszenie jest możliwe
        }
        else if (rozgrywka->oszustwa && IsType<Testpower>(r))
        {
            auto testpoweruj = (Testpower*)r;
            if (testpoweruj->kogo->poziom == 0) testpoweruj->kogo->typdomku = TypDomku::kMiasto;
            rozgrywka->ZmienPoziom(*testpoweruj->kogo, 5);
            rozgrywka->ZmienLiczebnosc(*testpoweruj->kogo, 1600.0);
        }
        //DYNAMICZNA PREDKOSC GRY
        else if (rozgrywka->oszustwa&&IsType<AktualizujPredkosc>(r))//TUTAJ TRZEBA SPRAWDZAC CZY NIE JEST MULTIPLAYER BO WTEDY ZMIANA PREDKOSCI BEDZIE ZMIENIAC TYLKO LOKALNIE
        {
            //aktualizuj->kto_wydal_rozkaz mowi ktory gracz to zrobil
            auto aktualizuj = (AktualizujPredkosc*)r;
            if (aktualizuj->wteczywewte == '+')szybkosc+=1.0;
            else if(szybkosc>1.0)szybkosc -= 1.0;

        }
        else if (IsType<ZmianaWidoku>(r)) {
            auto zmiana = (ZmianaWidoku*)r;
            sf::View& widok = zmiana->widok, & bazowy = zmiana->bazowy;
            sf::Vector2f dziesiata_czesc = { (float)(widok.getSize().x / 10.0), (float)(widok.getSize().y / 10.0) }, kursor = zmiana->kursor,
                minimalny = widok.getSize(), maksymalny = bazowy.getSize() - widok.getSize();
            kursor.x = max(min(bazowy.getSize().x - widok.getSize().x / 2.f, kursor.x), widok.getSize().x / 2.f);
            kursor.y = max(min(bazowy.getSize().y - widok.getSize().y / 2.f, kursor.y), widok.getSize().y / 2.f);
            if (zmiana->kierunek == 'X') {
                int tikiKola = zmiana->liczba_klikow_kola;

                int czesc_widoku = (int)(widok.getSize().x / (bazowy.getSize().x / 10.f));
                tikiKola = (tikiKola >= 0 ? min(10 - czesc_widoku, tikiKola) : max(1 - czesc_widoku, tikiKola));

                sf::Vector2f przesuniecie = (tikiKola < 0 ? kursor : bazowy.getCenter()) - widok.getCenter(), zmiana_rozmiaru = (bazowy.getSize() / 10.f) * (float)tikiKola;

                int x = (signbit(przesuniecie.x) ? -1 : 1), y = (signbit(przesuniecie.y) ? -1 : 1);

                if (sqrt(przesuniecie.x * przesuniecie.x + przesuniecie.y * przesuniecie.y) > abs((float)tikiKola) * sqrt(dziesiata_czesc.x * dziesiata_czesc.x + dziesiata_czesc.y * dziesiata_czesc.y)) {
                    przesuniecie = { (signbit(przesuniecie.x) ? -1 : 1) * dziesiata_czesc.x, (signbit(przesuniecie.y) ? -1 : 1) * dziesiata_czesc.y };
                    przesuniecie *= abs((float)tikiKola);
                }

                widok.setSize(widok.getSize() + zmiana_rozmiaru);
                widok.move(przesuniecie);
            }
            else {
                sf::Vector2f minimalny = widok.getSize() / 2.f, maxymalny = bazowy.getSize() - widok.getSize() / 2.f;
                if (zmiana->kierunek == 'W') {
                    if (widok.getCenter().y - dziesiata_czesc.y <= minimalny.y) widok.setCenter(widok.getCenter().x, minimalny.y);
                    else widok.move({ 0.f, -dziesiata_czesc.y });
                }
                if (zmiana->kierunek == 'S') {
                    if (widok.getCenter().y + dziesiata_czesc.y >= maxymalny.y) widok.setCenter(widok.getCenter().x, maxymalny.y);
                    else widok.move({ 0.f, dziesiata_czesc.y });
                    }
                if (zmiana->kierunek == 'A') {
                    if (widok.getCenter().x - dziesiata_czesc.x <= minimalny.x) widok.setCenter(minimalny.x, widok.getCenter().y);
                    else widok.move({ -dziesiata_czesc.x, 0.f });
                    }
                if (zmiana->kierunek == 'D' ) {
                    if (widok.getCenter().x + dziesiata_czesc.x >= maxymalny.x) widok.setCenter(maxymalny.x, widok.getCenter().y);
                    else widok.move({ dziesiata_czesc.x, 0.f });
                    }
                
            }
        }
        else if (IsType<CofajLudka>(r))//rozkaz cofania
        {
            auto cofanie = (CofajLudka*)r;

            if (!rozgrywka->oszustwa && cofanie->cofany->gracz->numer != cofanie->kto_wydal_rozkaz->numer)continue;
            
            Ludek* ludzik = cofanie->cofany;
            Twor* powrot = ludzik->cel;
            ludzik->cel = ludzik->skad;
            ludzik->skad = powrot;
        }
    }
    WyczyscKolejkeRuchow();
}

void Ruszacz::PrzesuwajLudkow(double czas)
{
    double przesuniecie = szybkosc * szybkosc_ruchu;
    ile_armii_idzie = rozgrywka->armie.size();
    for (Ludek& armia : rozgrywka->armie)
    {
        for (Ludek& mijana : rozgrywka->armie) {
            if (mijana.cel == armia.skad&& mijana.skad == armia.cel) {
                PD wektor_pomiedzy = armia.polozenie - mijana.polozenie;
                double odleglosc = sqrt(wektor_pomiedzy.x * wektor_pomiedzy.x + wektor_pomiedzy.y * wektor_pomiedzy.y);
                if (odleglosc <= przesuniecie * czas*(armia.szybkosc_ludka + mijana.szybkosc_ludka)) {
                    PD miejsce_spotkania = mijana.szybkosc_ludka/ (armia.szybkosc_ludka+mijana.szybkosc_ludka) * wektor_pomiedzy + mijana.polozenie;
                    armia.polozenie = miejsce_spotkania;
                    mijana.polozenie = miejsce_spotkania;
                    }
                }
            }
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

            if (przesuniecie*armia.szybkosc_ludka*czas >= dlugosc) {
                armia.polozenie = armia.cel->polozenie;
                armia.dystans += dlugosc;
            }
            else {
                armia.polozenie += jednostkowy;
                armia.dystans += sqrt(jednostkowy.x * jednostkowy.x + jednostkowy.y * jednostkowy.y);
            }
            
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
        if (spotkanie != NULL && armia.uid >= spotkanie->uid)
            continue;

        if (spotkanie != NULL && spotkanie->gracz == armia.gracz)
        {
            if (spotkanie->liczebnosc <= armia.liczebnosc)
            {
                rozgrywka->ZmienLiczebnosc(armia, armia.liczebnosc + spotkanie->liczebnosc);
                armia.tarcza += spotkanie->tarcza;
                do_usuniecia.push_back(spotkanie);
            }
            else
            {
                rozgrywka->ZmienLiczebnosc(*spotkanie, armia.liczebnosc + spotkanie->liczebnosc);
                spotkanie->tarcza += armia.tarcza;
                do_usuniecia.push_back(&(*it));
            }
        }
        else if (spotkanie != NULL)
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
        else if (odleglosc < min(armia.cel->rozmiar,armia.rozmiar))
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
        rozgrywka->ZniszczTwor(usunieta);
    }
}

void Ruszacz::Produkuj(double czas)
{
    for (Domek& domek : rozgrywka->domki)
    {
        if(rozgrywka->cel_gry.nazwa_celu == "Inwazja" && rozgrywka->cel_gry.nr_impostera == domek.gracz->numer && domek.gracz->aktywny && domek.liczebnosc < domek.max_liczebnosc)
            rozgrywka->ZmienLiczebnosc(domek, domek.liczebnosc + (double)rozgrywka->cel_gry.szybkosc_impostera * szybkosc * czas*domek.produkcja*domek.poziom);
        else if (domek.liczebnosc > domek.max_liczebnosc)rozgrywka->ZmienLiczebnosc(domek, max(domek.liczebnosc - szybkosc * czas*(domek.liczebnosc - domek.max_liczebnosc) / 10.0, (double)domek.max_liczebnosc));
        else if (rozgrywka->cel_gry.nazwa_celu == "Przetrwanie" && rozgrywka->Graczu(rozgrywka->cel_gry.nr_defensora).aktywny && domek.gracz->aktywny && rozgrywka->cel_gry.nr_defensora != domek.gracz->numer && domek.liczebnosc > 0.0) {
            if (domek.liczebnosc - szybkosc * czas*(double)rozgrywka->cel_gry.sila_plagi <= 0.0) {
                domek.poziom = 0;
                domek.typdomku = TypDomku::kPole;
                domek.max_liczebnosc = 0;
                rozgrywka->ZmienLiczebnosc(domek, 0.0);
            }
            else rozgrywka->ZmienLiczebnosc(domek, domek.liczebnosc - szybkosc * czas*(double)rozgrywka->cel_gry.sila_plagi);
        }
        else if (domek.gracz->aktywny&&domek.liczebnosc == domek.max_liczebnosc);
        else if (domek.gracz->aktywny&&domek.liczebnosc < domek.max_liczebnosc&&domek.typdomku == TypDomku::kMiasto)rozgrywka->ZmienLiczebnosc(domek, domek.liczebnosc + szybkosc * czas*domek.produkcja*domek.poziom);
    }
}

void Ruszacz::Strzelaj()
{
    vector<Ludek*> do_usuniecia;
    for (Ludek& ludek : rozgrywka->armie) {
        Domek* domek_cel = ((Domek*)ludek.cel);
        if (domek_cel->typdomku == TypDomku::kWieza&&domek_cel->gracz->numer != ludek.gracz->numer) {
            if ((ludek.dystans / ludek.droga)*((double)(domek_cel->poziom) / 6.0) >= 1.0 / 1000.0) {
                rozgrywka->TracLudki(ludek, (ludek.dystans / ludek.droga)*((double)(domek_cel->poziom) / 6.0)*ludek.liczebnosc);
                if (ludek.liczebnosc==0.0)do_usuniecia.push_back(&ludek);
                ludek.dystans = 0.0;
            }
        }
   }
    for (auto usunieta : do_usuniecia)
    {
        rozgrywka->ZniszczTwor(usunieta);
    }
}

void Ruszacz::WyczyscKolejkeRuchow()
{
    for (auto ruch : kolejka_do_wykonania)
        delete ruch;
    kolejka_do_wykonania.clear();
}

WymarszRozkaz::WymarszRozkaz()
{
}

WymarszRozkaz::WymarszRozkaz(Domek * skad, Domek * dokad) : Rozkaz(skad->gracz), skad(skad), dokad(dokad)
{
}

WymarszRozkaz::WymarszRozkaz(Domek * skad, Domek * dokad, Gracz& kto_wydal_rozkaz) : Rozkaz(&kto_wydal_rozkaz), skad(skad), dokad(dokad)
{
}

UlepszRozkaz::UlepszRozkaz()
{
}

UlepszRozkaz::UlepszRozkaz(Domek * kogo) : Rozkaz(kogo->gracz), kogo(kogo)
{
}

UlepszRozkaz::UlepszRozkaz(Domek * kogo, Gracz& kto_wydal_rozkaz) : Rozkaz(&kto_wydal_rozkaz), kogo(kogo)
{
}

BurzRozkaz::BurzRozkaz()
{
}

BurzRozkaz::BurzRozkaz(Domek * kogo) : Rozkaz(kogo->gracz), kogo(kogo)
{
}

BurzRozkaz::BurzRozkaz(Domek * kogo, Gracz& kto_wydal_rozkaz) : Rozkaz(&kto_wydal_rozkaz), kogo(kogo)
{
}

PrzebudujRozkaz::PrzebudujRozkaz()
{
}

PrzebudujRozkaz::PrzebudujRozkaz(Domek * kogo, TypDomku naco) : Rozkaz(kogo->gracz), kogo(kogo), naco(naco)
{
}

PrzebudujRozkaz::PrzebudujRozkaz(Domek * kogo, TypDomku naco, Gracz& kto_wydal_rozkaz) : Rozkaz(&kto_wydal_rozkaz), kogo(kogo), naco(naco)
{
}

Testpower::Testpower(Domek * kogo) : Rozkaz(kogo->gracz), kogo(kogo)
{
}

AktualizujPredkosc::AktualizujPredkosc(char wteczywewte, Gracz& kto_taki_cwany) : Rozkaz(&kto_taki_cwany), wteczywewte(wteczywewte)
{
}

CofajLudka::CofajLudka() 
{
}

CofajLudka::CofajLudka(Ludek* cofany, Gracz& cofajacy) : Rozkaz(&cofajacy), cofany(cofany)
{
}

ZmianaWidoku::ZmianaWidoku(char kierunek, Gracz& kto_taki_cwany, sf::View& widok, sf::View& bazowy, sf::Vector2f kursor, int liczba_klikow) : Rozkaz(&kto_taki_cwany), kierunek(kierunek), widok(widok), bazowy(bazowy), kursor(kursor), liczba_klikow_kola(liczba_klikow)
{
}


