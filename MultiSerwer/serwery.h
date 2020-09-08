#pragma once

#include <iterator>

#include "../Domki/ext_string.h"
#include "../Domki/multi.h"
#include "../Domki/multi_dzialacze.h"

#include "easylogging++.h" 

using namespace std;
using namespace multi;

class KontekstGry {
public:
    Serwer* serwer = NULL;
    Klient* klient = NULL;
    MisjaUstawienia misja_ustawienia;

    static KontekstGry* obiekt;
    static KontekstGry& o() {
        if (obiekt == NULL)
            obiekt = new KontekstGry();
        return *obiekt;

    }
};

void komunikat_serwer_gry(); 
void komunikat_serwer_klient();

void start_serwer_gry(string zadanie);
void wykonaj_serwer_gry(string zadanie);

void start_klient_gry(string zadanie);
void wykonaj_klient_gry(string zadanie);
void pobierz_serwery_gry();


