#pragma once

#include <thread>

#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <iostream>
#include "easylogging++.h"

#include "gui.h"
#include "os.h"
#include "multi.h"
#include "../MultiSerwer/mastery.h"

// MECHANIZM

string start_generuj_komende_startu(string folder, string mapa, int do_ilu, double szybkosc, int ile_ludzi, int port_serwera);
PROCESS_INFORMATION start_nowej_gry_dla_wielu(string parametry);
void start_klient(sf::Music& muzyka, string nazwa, multi::Adres adres=multi::Adres());

// GUI

std::shared_ptr<sfg::Window> serwer_menu_tutaj(std::shared_ptr<sfg::Window> glowne, sf::Music& muzyka, string nazwa);
std::shared_ptr<sfg::Window> serwer_menu_planeta(std::shared_ptr<sfg::Window> glowne, sf::Music& muzyka, mastery::Klient* master_klient);

std::shared_ptr<sfg::Window> start_serwer_menu(std::shared_ptr<sfg::Window> glowne, sf::Music& muzyka, string nazwa, function<void(string)> zakladaj_fun);
std::shared_ptr<sfg::Window> wielu_graczy_menu(std::shared_ptr<sfg::Window> glowne, sf::Music& muzyka);