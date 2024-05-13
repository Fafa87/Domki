#include "decydent_edytora.h"

float szerokosc_kraty = 80.0, wysokosc_kraty = 90.0;

DecydentEdytor::DecydentEdytor(sf::RenderWindow& okno, Rozgrywka& rozgrywka) : okno(okno), rozgrywka(rozgrywka)
{
    resetuj();
}

void DecydentEdytor::resetuj() {
    wybrany = NULL;
    tworzony.liczebnosc = -1;
    tworz = false;
    laczenie_miast = false;
    ladowanie_ludkow = 0;
    pierwszy = NULL;
    drugi = NULL;
}

sf::Vector2f ustaw_pozycje(sf::Vector2f polozenie_kliku, float szerokosc_podzialu, float wysokosc_podzialu) {
    sf::Vector2f koryguj;
    koryguj.x = (float)((int)polozenie_kliku.x / (int)szerokosc_podzialu) * szerokosc_podzialu + szerokosc_podzialu / 2.0;
    koryguj.y = (float)((int)polozenie_kliku.y / (int)wysokosc_podzialu) * wysokosc_podzialu + wysokosc_podzialu / 2.0;
    return koryguj;
}

void rozstaw_domki(Rozgrywka & rozgrywka) {
    for (Domek& domek : rozgrywka.domki) {
        sf::Vector2f stare;
        stare.x = domek.polozenie.x;
        stare.y = domek.polozenie.y;
        sf::Vector2f nowe = ustaw_pozycje(stare, szerokosc_kraty, wysokosc_kraty);
        domek.polozenie.x = nowe.x;
        domek.polozenie.y = nowe.y;
    }

}

void DecydentEdytor::Przetworz(sf::Event zdarzenie) {
    if (zdarzenie.type == sf::Event::MouseButtonPressed) {

        sf::Vector2i pixelPos = sf::Mouse::getPosition(okno);
        miejsce_tworzenia = ustaw_pozycje(okno.mapPixelToCoords(pixelPos), szerokosc_kraty, wysokosc_kraty);
        Twor* klikniety = rozgrywka.Zlokalizuj(miejsce_tworzenia.x, miejsce_tworzenia.y);

        if (zdarzenie.mouseButton.button == sf::Mouse::Left) {
            if (laczenie_miast && IsType<Domek>(klikniety)) {
                if (pierwszy == NULL) pierwszy = (Domek*)klikniety;
                else drugi = (Domek*)klikniety;
            }
            else wybrany = klikniety;
        }

        else if (tworzony.liczebnosc != -1 && klikniety == NULL && zdarzenie.mouseButton.button == sf::Mouse::Right) {
            tworz = true;
        }
    }

    else if (zdarzenie.type == sf::Event::KeyPressed) {

        switch (zdarzenie.key.code) {
        case sf::Keyboard::Backspace: {
            if (wybrany != NULL) {
                for (Domek& domek : rozgrywka.domki) {
                    for (std::vector<Domek*>::iterator elem = domek.drogi.begin(); elem != domek.drogi.end(); elem++) {
                        if ((*elem) == wybrany) {
                            domek.drogi.erase(elem);
                            break;
                        }
                    }
                }
                rozgrywka.ZniszczTwor(wybrany);
                wybrany = NULL;
            }
            break;
        }
        case sf::Keyboard::Num0: {

            tworzony.liczebnosc = -1;
            rozgrywka.pozostale.remove(&tworzony);
            break;
        }
        case sf::Keyboard::Tilde: {

            if (tworzony.liczebnosc == -1) {
                tworzony = rozgrywka.stworz_domyslny_domek();
                tworzony.typdomku = TypDomku::kPole;
                rozgrywka.ZmienPoziom(tworzony, 0);
                rozgrywka.pozostale.push_back(&tworzony);
            }
            tworzony.typdomku = TypDomku::kPole;
            rozgrywka.ZmienPoziom(tworzony, 0);

            break;
        }
        case sf::Keyboard::Num1: {

            if (tworzony.liczebnosc == -1) {
                tworzony = rozgrywka.stworz_domyslny_domek();
                tworzony.typdomku = TypDomku::kMiasto;
                rozgrywka.pozostale.push_back(&tworzony);
            }
            if (tworzony.poziom)tworzony.typdomku = TypDomku::kMiasto;

            break;
        }
        case sf::Keyboard::Num2: {

            if (tworzony.liczebnosc == -1) {
                tworzony = rozgrywka.stworz_domyslny_domek();
                tworzony.typdomku = TypDomku::kFort;
                rozgrywka.pozostale.push_back(&tworzony);
            }
            if (tworzony.poziom)tworzony.typdomku = TypDomku::kFort;

            break;
        }
        case sf::Keyboard::Num3: {

            if (tworzony.liczebnosc == -1) {
                tworzony = rozgrywka.stworz_domyslny_domek();
                tworzony.typdomku = TypDomku::kZbrojownia;
                rozgrywka.pozostale.push_back(&tworzony);
            }
            if (tworzony.poziom)tworzony.typdomku = TypDomku::kZbrojownia;

            break;
        }
        case sf::Keyboard::Num4: {

            if (tworzony.liczebnosc == -1) {
                tworzony = rozgrywka.stworz_domyslny_domek();
                tworzony.typdomku = TypDomku::kWieza;
                rozgrywka.pozostale.push_back(&tworzony);
            }
            if (tworzony.poziom)tworzony.typdomku = TypDomku::kWieza;

            break;
        }
        case sf::Keyboard::Num5: {

            if (tworzony.liczebnosc == -1) {
                tworzony = rozgrywka.stworz_domyslny_domek();
                tworzony.typdomku = TypDomku::kStajnia;
                rozgrywka.pozostale.push_back(&tworzony);
            }
            if (tworzony.poziom)tworzony.typdomku = TypDomku::kStajnia;

            break;
        }
        case sf::Keyboard::Numpad0: {

            if (tworzony.liczebnosc == -1) {
                tworzony = rozgrywka.stworz_domyslny_domek();
                rozgrywka.pozostale.push_back(&tworzony);
            }
            tworzony.gracz = &rozgrywka.Graczu(0);

            break;
        }
        case sf::Keyboard::Numpad1: {

            if (tworzony.liczebnosc == -1) {
                tworzony = rozgrywka.stworz_domyslny_domek();
                rozgrywka.pozostale.push_back(&tworzony);
            }
            if (rozgrywka.gracze.size() > 1)tworzony.gracz = &rozgrywka.Graczu(1);
            else tworzony.gracz = &rozgrywka.Graczu(0);

            break;
        }
        case sf::Keyboard::Numpad2: {

            if (tworzony.liczebnosc == -1) {
                tworzony = rozgrywka.stworz_domyslny_domek();
                rozgrywka.pozostale.push_back(&tworzony);
            }
            if (rozgrywka.gracze.size() > 2)tworzony.gracz = &rozgrywka.Graczu(2);
            else tworzony.gracz = &rozgrywka.Graczu(0);

            break;
        }
        case sf::Keyboard::Numpad3: {

            if (tworzony.liczebnosc == -1) {
                tworzony = rozgrywka.stworz_domyslny_domek();
                rozgrywka.pozostale.push_back(&tworzony);
            }
            if (rozgrywka.gracze.size() > 3)tworzony.gracz = &rozgrywka.Graczu(3);
            else tworzony.gracz = &rozgrywka.Graczu(0);

            break;
        }
        case sf::Keyboard::Numpad4: {

            if (tworzony.liczebnosc == -1) {
                tworzony = rozgrywka.stworz_domyslny_domek();
                rozgrywka.pozostale.push_back(&tworzony);
            }
            if (rozgrywka.gracze.size() > 4)tworzony.gracz = &rozgrywka.Graczu(4);
            else tworzony.gracz = &rozgrywka.Graczu(0);
            break;
        }
        case sf::Keyboard::Numpad5: {

            if (tworzony.liczebnosc == -1) {
                tworzony = rozgrywka.stworz_domyslny_domek();
                rozgrywka.pozostale.push_back(&tworzony);
            }
            if (rozgrywka.gracze.size() > 5)tworzony.gracz = &rozgrywka.Graczu(5);
            else tworzony.gracz = &rozgrywka.Graczu(0);

            break;
        }
        case sf::Keyboard::Numpad6: {

            if (tworzony.liczebnosc == -1) {
                tworzony = rozgrywka.stworz_domyslny_domek();
                rozgrywka.pozostale.push_back(&tworzony);
            }
            if (rozgrywka.gracze.size() > 6)tworzony.gracz = &rozgrywka.Graczu(6);
            else tworzony.gracz = &rozgrywka.Graczu(0);

            break;
        }
        case sf::Keyboard::Numpad7: {

            if (tworzony.liczebnosc == -1) {
                tworzony = rozgrywka.stworz_domyslny_domek();
                rozgrywka.pozostale.push_back(&tworzony);
            }
            if (rozgrywka.gracze.size() > 7)tworzony.gracz = &rozgrywka.Graczu(7);
            else tworzony.gracz = &rozgrywka.Graczu(0);

            break;
        }
        case sf::Keyboard::Numpad8: {

            if (tworzony.liczebnosc == -1) {
                tworzony = rozgrywka.stworz_domyslny_domek();
                rozgrywka.pozostale.push_back(&tworzony);
            }
            if (rozgrywka.gracze.size() > 8) tworzony.gracz = &rozgrywka.Graczu(8);
            else tworzony.gracz = &rozgrywka.Graczu(0);

            break;
        }
        case sf::Keyboard::Numpad9: {

            if (tworzony.liczebnosc == -1) {
                tworzony = rozgrywka.stworz_domyslny_domek();
                rozgrywka.pozostale.push_back(&tworzony);
            }
            if (rozgrywka.gracze.size() > 9) tworzony.gracz = &rozgrywka.Graczu(9);
            else tworzony.gracz = &rozgrywka.Graczu(0);

            break;
        }
        case sf::Keyboard::Add: {

            if (tworzony.liczebnosc == -1) {
                tworzony = rozgrywka.stworz_domyslny_domek();
                rozgrywka.pozostale.push_back(&tworzony);
            }
            if (tworzony.poziom < 5) {
                rozgrywka.ZmienPoziom(tworzony, tworzony.poziom + 1);
            }

            break;
        }
        case sf::Keyboard::Subtract: {

            if (tworzony.liczebnosc == -1) {
                tworzony = rozgrywka.stworz_domyslny_domek();
                rozgrywka.pozostale.push_back(&tworzony);
            }
            if (tworzony.poziom > 0) {
                rozgrywka.ZmienPoziom(tworzony, tworzony.poziom - 1);
            }

            break;
        }
        case sf::Keyboard::Tab: {
            if (!ladowanie_ludkow)ladowanie_ludkow = clock();
            break;
        }
        case sf::Keyboard::S: {
            zapisuj.first = true;
            break;
        }
        case sf::Keyboard::O: {
            otwieraj.first = true;
            break;
        }
        case sf::Keyboard::LControl: {
            zapisuj.second = true;
            otwieraj.second = true;
            break;
        }
        case sf::Keyboard::RControl: {
            zapisuj.second = true;
            otwieraj.second = true;
            break;
        }
        case sf::Keyboard::LAlt: {
            laczenie_miast = true;
            break;
        }
        case sf::Keyboard::RAlt: {
            laczenie_miast = true;
            break;
        }
        }
    }

    else if (zdarzenie.type == sf::Event::KeyReleased && zdarzenie.key.code == sf::Keyboard::Tab) {

        if (tworzony.liczebnosc == -1) {
            tworzony = rozgrywka.stworz_domyslny_domek();
            rozgrywka.pozostale.push_back(&tworzony);
        }
        if (ladowanie_ludkow) ladowanie_ludkow = 0;
        else if (tworzony.typdomku == TypDomku::kPole) tworzony.liczebnosc = 0;
        else {
            if (tworzony.liczebnosc < tworzony.max_liczebnosc / 2.0) tworzony.liczebnosc = tworzony.max_liczebnosc / 2.0;
            else if (tworzony.liczebnosc < tworzony.max_liczebnosc) tworzony.liczebnosc = tworzony.max_liczebnosc;
            else tworzony.liczebnosc = 0.0;
        }
    }

    else if (zdarzenie.type == sf::Event::KeyReleased && zapisuj.first && zapisuj.second && (zdarzenie.key.code == sf::Keyboard::S || zdarzenie.key.code == sf::Keyboard::LControl || zdarzenie.key.code == sf::Keyboard::RControl)) {
        string sciezka = open_folder_from_folder("");
        zapis_mapy(rozgrywka, sciezka);
        zapisuj.first = false;
        zapisuj.second = false;
    }
    else if (zdarzenie.type == sf::Event::KeyReleased && otwieraj.first && otwieraj.second && (zdarzenie.key.code == sf::Keyboard::O || zdarzenie.key.code == sf::Keyboard::LControl || zdarzenie.key.code == sf::Keyboard::RControl)) {
        string sciezka = open_file_from_folder("");
        if (sciezka != "") {
            resetuj();
            rozgrywka = zwarcie_rozgrywka(sciezka);
        }
        rozstaw_domki(rozgrywka); 
        otwieraj.first = false;
        otwieraj.second = false;
    }
    else if (zdarzenie.type == sf::Event::KeyReleased && zdarzenie.key.code == sf::Keyboard::S) {
        zapisuj.first = false;
    }
    else if (zdarzenie.type == sf::Event::KeyReleased && zdarzenie.key.code == sf::Keyboard::O) {
        otwieraj.first = false;
    }
    else if (zdarzenie.type == sf::Event::KeyReleased && (zdarzenie.key.code == sf::Keyboard::LControl || zdarzenie.key.code == sf::Keyboard::RControl)) {
        zapisuj.second = false;
        otwieraj.second = false;
    }
    else if (zdarzenie.type == sf::Event::KeyReleased && (zdarzenie.key.code == sf::Keyboard::LAlt || zdarzenie.key.code == sf::Keyboard::RAlt)) {
        laczenie_miast = false;
    }
}

void DecydentEdytor::Wykonaj() {
    if (pierwszy != NULL && drugi != NULL) {
        if (pierwszy != drugi && laczenie_miast) {
			Domek* P = NULL, *D = NULL;
			for (Domek* drogowy : pierwszy->drogi)
				if (drogowy == drugi) P = drugi;
			for (Domek* drogowy : drugi->drogi)
				if (drogowy == pierwszy) D = pierwszy;
            if (P == NULL && D == NULL) rozgrywka.PolaczDomki(*pierwszy, *drugi);
        }

        pierwszy = NULL;
        drugi = NULL;
    }
    if (tworzony.liczebnosc != -1 && tworz) {

        tworzony.polozenie.x = miejsce_tworzenia.x;
        tworzony.polozenie.y = miejsce_tworzenia.y;
        tworzony.uid = tworzony.last_uid++;
        rozgrywka.DajTwor(&tworzony);
        rozgrywka.domki.push_back(tworzony);

        tworz = false;
    }
    else if (tworzony.liczebnosc != -1 && ladowanie_ludkow && (double)(clock() - ladowanie_ludkow) / CLOCKS_PER_SEC > 0.5) tworzony.liczebnosc += ((double)(clock() - ladowanie_ludkow) / CLOCKS_PER_SEC - 0.5) * tworzony.max_liczebnosc;
}

