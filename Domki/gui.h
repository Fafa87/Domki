#pragma once

#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include <SFML/Graphics/RenderWindow.hpp>

using namespace std;

namespace GUI
{
	static sfg::SFGUI sfgui;
	static sfg::Desktop pulpit;

	void center_window(sf::RenderWindow& parent, shared_ptr<sfg::Window> window);
	void bottom_window(sf::RenderWindow& parent, shared_ptr<sfg::Window> window);
	void bottom_left_window(sf::RenderWindow& parent, shared_ptr<sfg::Window> window);
}