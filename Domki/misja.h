#pragma once
#include <SFML/Graphics.hpp>
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Window.hpp>
#include <SFGUI/Table.hpp>

#include "dane.h"
#include "dzialacze.h"
#include "wyswietlacze.h"
#include "muzyka.h"
#include "si.h"
#include "os.h"

#include<windows.h>
#include<fstream>

struct MisjaUstawienia
{
    MisjaUstawienia();
    int Zwyciezca();
    bool JedenGracz();
    void WypiszRanking();

    string grupa = "Plansza";
    string nazwa = "";
    double szybkosc = 1.5;
    double trudnosc = 5.0;
    string skorka = "rycerze_hd";

    bool oszustwa = false,
        walka_w_polu = true,
        punkty_kontrolne = true;

    CelMisji cel_misji;

    int nr_gracza = 1;
    vector<string> nazwy_graczow;
    vector<int> komputery = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };

    string nazwy[3][10] = { {"Cienias","Ludek","Kapuniak","Placek","Amator","Maniak","Rycerz","Mistrz","Mocarz","Cezar"},
                                       {"Leszcz","Klocek","Lamer","Strudel","Miedziak","Pasjonat","Wojownik","Zawodowiec","Masakrator","Dipblu"} ,
                                        {"Wsiok","Klapek","Bober","Klopsik","Cwaniak","Znawca","Walczak","Ekspert","Magnat","Mutant"} };
    int do_ilu_wygranych = 0;
    vector<int> ile_kto_wygranych;
};

vector<string> wczytaj_liste_plansz(string folder);

vector<string> wczytaj_liste_folderow(string folder);

MisjaUstawienia wczytaj_meta(string sciezka);

void zapis_mapy(const Rozgrywka& konstrukcja, string sciezka);

Rozgrywka zwarcie_rozgrywka(string sciezka);

string map_generator(int liczba_graczy);

int misja(MisjaUstawienia& misja_ustawienia, Ruszacz& ruszacz);

int misja(MisjaUstawienia& misja_ustawienia);


sf::View wysrodkowany_widok(list<Domek> &domki, int bottom_space = 0);

shared_ptr<sfg::Window> interfejs_rozgrywki(shared_ptr<sfg::Window> interfejs, MisjaUstawienia &stan, Rozgrywka& rozgrywka, Wyswietlacz& wyswietlacz, Domek* wybrany, Twor* skupiony);

shared_ptr<sfg::Window> pokazowy_interfejs(shared_ptr<sfg::Window> interfejs, MisjaUstawienia &stan, Rozgrywka& rozgrywka, Wyswietlacz& wyswietlacz, Domek* wybrany, Twor* skupiony);

shared_ptr<sfg::Table> interfejs_ranking(MisjaUstawienia &stan, Rozgrywka& rozgrywka, int instance);

bool odliczanie(int czas, Wyswietlacz& wyswietlacz, sf::View widok, std::shared_ptr<sfg::Window> gui_pasek, Ruszacz& ruszacz);

void zakonczenie_gry(Gracz& gracz_wygrany, int grajacy);

void zakonczenie_meczu(MisjaUstawienia &stan, Rozgrywka& rozgrywka);