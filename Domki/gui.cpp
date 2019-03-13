#include "gui.h"
#include "windows.h"

GUI GUI::aplikacja;

void GUI::setup_theme()
{
	pulpit.LoadThemeFromFile("Grafika\\bazowy.theme");

	pulpit.SetProperty("Window", "BackgroundColor", sf::Color(6, 4, 50));
	pulpit.SetProperty("Label", "Color", sf::Color::White);
	pulpit.SetProperty("Label", "FontSize", 28);
	pulpit.SetProperty("Label#Naglowek", "FontSize", 48);
	pulpit.SetProperty("Label#Alarm", "FontSize", 80);
	pulpit.SetProperty("Label#Alarm", "Color", sf::Color::Yellow);
	pulpit.SetProperty("ComboBox", "BackgroundColor", sf::Color(0, 50, 120));
	pulpit.SetProperty("Button", "BackgroundColor", sf::Color(0, 50, 120));
}

void GUI::center_window(shared_ptr<sfg::Window> window)
{
	auto win_rect = window->GetAllocation();
	sf::Vector2f size(win_rect.width, win_rect.height);
	auto parent_size = (sf::Vector2f)okno.getSize();
	window->SetPosition((parent_size - size) / 2.0f);
}

void GUI::bottom_window(shared_ptr<sfg::Window> window)
{
	auto win_rect = window->GetAllocation();
	sf::Vector2f size(win_rect.width, win_rect.height);
	auto final_position = (sf::Vector2f)okno.getSize();
	final_position.y -= size.y;
	final_position.x = (final_position.x - size.x) / 2;
	window->SetPosition(final_position);
}

void GUI::bottom_left_window(shared_ptr<sfg::Window> window)
{
	auto win_rect = window->GetAllocation();
	sf::Vector2f size(win_rect.width, win_rect.height);
	auto final_position = (sf::Vector2f)okno.getSize();
	final_position.y -= size.y;
	final_position.x = 0;
	window->SetPosition(final_position);
}

void GUI::wait_for_anything()
{
	sf::Event event;
	while (true)
	{
		okno.pollEvent(event);
		if (event.type == sf::Event::KeyReleased || event.type == sf::Event::MouseButtonReleased)
			break;
		Sleep(100);
	}
}

void GUI::show_and_wait_for_anything(shared_ptr<sfg::Window> window)
{
	set_active_window(window);
	center_window(window);

	pulpit.Update(1);
	sfgui.Display(okno);
	okno.display();

	wait_for_anything();

	remove_active_window(window);
}

void GUI::set_active_window(shared_ptr<sfg::Window> window)
{
	pulpit.Add(window);
	window->Show(true);
	windows.push_back(window);
}

void GUI::remove_active_window(shared_ptr<sfg::Window> window)
{
	pulpit.Remove(window);
	window->Show(false);
	windows.erase(std::remove(windows.begin(), windows.end(), window), windows.end());

	if (windows.size() > 0)
		set_active_window(windows.back());
}

void GUI::show_all_windows()
{
	for (auto& w : windows)
		w->Show(true);
}

void GUI::hide_all_windows()
{
	for (auto& w : windows)
		w->Show(false);
}
