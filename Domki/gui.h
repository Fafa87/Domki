#pragma once

#include <thread>
#include <windows.h>

#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "Narzedzia/INIReader.h"

using namespace std;

class GUI
{
private:
    sf::Sprite nico;
    static int styl_okna() 
    {
        auto res = sf::Style::Fullscreen;
        #if defined(_DEBUG) || defined(_WINDOW)
        res = sf::Style::Default;
        #endif
        return res;
    }

    static sf::ContextSettings ustawienia()
    {
        sf::ContextSettings res;
        res.antialiasingLevel = 8;
        return res;
    }

    static sf::VideoMode ekran()
    {
        return sf::VideoMode(1280, 719);
    }
public:
    GUI() : okno(ekran(), "Domki Forever!", styl_okna(), ustawienia()), ini("Domki.conf") {
        okno.resetGLStates();
        zwykly_widok = sf::View(sf::FloatRect(0, 0, 1280, 719));
        okno.setView(zwykly_widok);

        sf::Texture obrazek_tla;
        obrazek_tla.create(10, 10);
        nico.setTexture(obrazek_tla);
    }

    sfg::SFGUI sfgui;
    sfg::Desktop pulpit;
    sf::RenderWindow okno;
    sf::View zwykly_widok;
    vector<shared_ptr<sfg::Window>> windows;
    vector<PROCESS_INFORMATION> zalozone_gry;
    int dzwieki_glosnosc = 100;

    INIReader ini;

    void setup_theme();
    void center_window(shared_ptr<sfg::Window> window);
    void bottom_window(shared_ptr<sfg::Window> window);
    void put_right_to(shared_ptr<sfg::Window> window, int startFromX);
    void bottom_left_window(shared_ptr<sfg::Window> window);
    void top_right_window(shared_ptr<sfg::Window> window);

    void set_viewport_abs(sf::View& view, sf::FloatRect rect);
    void show_bottom_gui(sf::View& view, std::shared_ptr<sfg::Window> gui_okno);
    void finish_viewport_render(sf::View& view);

    void stretch_up_down(shared_ptr<sfg::Window> window);

    void show_and_wait_for_anything(shared_ptr<sfg::Window> window);
    void wait_for_anything();

    void set_active_window(shared_ptr<sfg::Window> window);
    void remove_active_window(shared_ptr<sfg::Window> window);
    void pop_active_window(shared_ptr<sfg::Window> window);
    void show_all_windows();
    void hide_all_windows();

    void render();
    void reset_view();

    static GUI* apa;
    static GUI& aplikacja() {
        if (apa == NULL)
            apa = new GUI();
        return *apa;

    }
};