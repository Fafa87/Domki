#include "gui.h"

void GUI::center_window(sf::RenderWindow& parent, shared_ptr<sfg::Window> window)
{
	auto win_rect = window->GetAllocation();
	sf::Vector2f size(win_rect.width, win_rect.height);
	auto parent_size = (sf::Vector2f)parent.getSize();
	window->SetPosition((parent_size - size) / 2.0f);
}