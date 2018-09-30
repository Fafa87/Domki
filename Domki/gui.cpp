#include "gui.h"

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