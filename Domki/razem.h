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

// MECHANIZM

PROCESS_INFORMATION start_nowej_gry_dla_wielu(string folder, string mapa, int do_ilu, double szybkosc, int ile_ludzi);
void start_klient(sf::Music& muzyka, string nazwa, multi::Adres adres=multi::Adres());

// GUI

std::shared_ptr<sfg::Window> start_serwer_menu(std::shared_ptr<sfg::Window> glowne, sf::Music& muzyka, string nazwa);
std::shared_ptr<sfg::Window> wielu_graczy_menu(std::shared_ptr<sfg::Window> glowne, sf::Music& muzyka);