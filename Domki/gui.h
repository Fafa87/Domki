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
	}

	sfg::SFGUI sfgui;
	sfg::Desktop pulpit;
	sf::RenderWindow okno;
	vector<shared_ptr<sfg::Window>> windows;

	void setup_theme();
	void center_window(shared_ptr<sfg::Window> window);
	void bottom_window(shared_ptr<sfg::Window> window);
	void bottom_left_window(shared_ptr<sfg::Window> window);

	void show_and_wait_for_anything(shared_ptr<sfg::Window> window);
	void wait_for_anything();

	void set_active_window(shared_ptr<sfg::Window> window);
	void remove_active_window(shared_ptr<sfg::Window> window);
	void show_all_windows();
	void hide_all_windows();

	static GUI aplikacja;
};