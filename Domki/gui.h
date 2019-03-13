#pragma once

#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

using namespace std;

class GUI
{
private:
	static int styl_okna() 
	{
		auto res = sf::Style::Fullscreen;
		#ifdef _DEBUG
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
	GUI() : okno(ekran(), "Domki Forever!", styl_okna(), ustawienia()) {
		okno.resetGLStates();
		zwykly_widok = sf::View(sf::FloatRect(0, 0, 1280, 719));
		okno.setView(zwykly_widok);
	}

	sfg::SFGUI sfgui;
	sfg::Desktop pulpit;
	sf::RenderWindow okno;
	sf::View zwykly_widok;
	vector<shared_ptr<sfg::Window>> windows;

	void setup_theme();
	void center_window(shared_ptr<sfg::Window> window);
	void bottom_window(shared_ptr<sfg::Window> window);
	void bottom_left_window(shared_ptr<sfg::Window> window);
	void top_right_window(shared_ptr<sfg::Window> window);

	void stretch_up_down(shared_ptr<sfg::Window> window);

	void show_and_wait_for_anything(shared_ptr<sfg::Window> window);
	void wait_for_anything();

	void set_active_window(shared_ptr<sfg::Window> window);
	void remove_active_window(shared_ptr<sfg::Window> window);
	void show_all_windows();
	void hide_all_windows();

	void render();
	void reset_view();

	static GUI aplikacja;
};