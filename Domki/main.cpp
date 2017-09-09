#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include <SFML/Graphics.hpp>

#include "misja.h"

int main() {

	// Create SFML's window.
	sf::RenderWindow render_window(sf::VideoMode(800, 600), "Domki menu!", sf::Style::None);
	sfg::SFGUI sfgui;

	// Create the label.
	auto label = sfg::Label::Create("Hello world!");

	// Create a simple button and connect the click signal.
	auto button = sfg::Button::Create("Greet SFGUI!");
	button->GetSignal(sfg::Widget::OnLeftClick).Connect([label] { 

		label->SetText("Hello SFGUI, pleased to meet you!"); 
		//render_window
		misja("Plansza\\tomek_test.txt");
	});

	// Create a vertical box layouter with 5 pixels spacing and add the label
	// and button to it.
	auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 5.0f);
	box->Pack(label);
	box->Pack(button, false);

	// Create a window and add the box layouter to it. Also set the window's title.
	auto window = sfg::Window::Create();
	window->SetTitle("Hello world!");
	window->Add(box);

	// Create a desktop and add the window to it.
	sfg::Desktop desktop;
	desktop.Add(window);

	// We're not using SFML to render anything in this program, so reset OpenGL
	// states. Otherwise we wouldn't see anything.
	render_window.resetGLStates();

	// Main loop!
	sf::Event event;
	sf::Clock clock;

	while (render_window.isOpen()) {
		// Event processing.
		while (render_window.pollEvent(event)) {
			desktop.HandleEvent(event);

			// If window is about to be closed, leave program.
			if (event.type == sf::Event::Closed) {
				return 0;
			}
		}

		// Update SFGUI with elapsed seconds since last call.
		desktop.Update(clock.restart().asSeconds());

		// Rendering.
		render_window.clear();
		sfgui.Display(render_window);
		render_window.display();
	}

	return 0;
}
