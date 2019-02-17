#pragma once

#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

using namespace std;

namespace GUI
{
	static sfg::SFGUI sfgui;
	static sfg::Desktop pulpit;
	static vector<shared_ptr<sfg::Window>> windows;

	void setup_theme();
	void set_active_window(shared_ptr<sfg::Window> window);
	void remove_active_window(shared_ptr<sfg::Window> window);
	void show_all_windows();
	void hide_all_windows();

	void center_window(sf::RenderWindow& parent, shared_ptr<sfg::Window> window);
	void bottom_window(sf::RenderWindow& parent, shared_ptr<sfg::Window> window);
	void bottom_left_window(sf::RenderWindow& parent, shared_ptr<sfg::Window> window);

	void wait_for_anything(sf::RenderWindow& window);
}