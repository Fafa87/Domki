#include "gui.h"
#include "windows.h"

void GUI::setup_theme()
{
	GUI::pulpit.LoadThemeFromFile("Grafika\\bazowy.theme");

	GUI::pulpit.SetProperty("Window", "BackgroundColor", sf::Color(6, 4, 50));
	GUI::pulpit.SetProperty("Label", "Color", sf::Color::White);
	GUI::pulpit.SetProperty("Label", "FontSize", 28);
	GUI::pulpit.SetProperty("Label#Naglowek", "FontSize", 48);
	GUI::pulpit.SetProperty("Label#Alarm", "FontSize", 80);
	GUI::pulpit.SetProperty("Label#Alarm", "Color", sf::Color::Yellow);
	GUI::pulpit.SetProperty("ComboBox", "BackgroundColor", sf::Color(0, 50, 120));
	GUI::pulpit.SetProperty("Button", "BackgroundColor", sf::Color(0, 50, 120));
}

void GUI::center_window(sf::RenderWindow& parent, shared_ptr<sfg::Window> window)
{
	auto win_rect = window->GetAllocation();
	sf::Vector2f size(win_rect.width, win_rect.height);
	auto parent_size = (sf::Vector2f)parent.getSize();
	window->SetPosition((parent_size - size) / 2.0f);
}

void GUI::bottom_window(sf::RenderWindow& parent, shared_ptr<sfg::Window> window)
{
	auto win_rect = window->GetAllocation();
	sf::Vector2f size(win_rect.width, win_rect.height);
	auto final_position = (sf::Vector2f)parent.getSize();
	final_position.y -= size.y;
	final_position.x = (final_position.x - size.x) / 2;
	window->SetPosition(final_position);
}

void GUI::bottom_left_window(sf::RenderWindow& parent, shared_ptr<sfg::Window> window)
{
	auto win_rect = window->GetAllocation();
	sf::Vector2f size(win_rect.width, win_rect.height);
	auto final_position = (sf::Vector2f)parent.getSize();
	final_position.y -= size.y;
	final_position.x = 0;
	window->SetPosition(final_position);
}

void GUI::wait_for_anything(sf::RenderWindow& window)
{
	sf::Event event;
	while (true)
	{
		window.pollEvent(event);
		if (event.type == sf::Event::KeyReleased || event.type == sf::Event::MouseButtonReleased)
			break;
		Sleep(100);
	}
}