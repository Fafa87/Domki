#include <SFML/Graphics.hpp>

int main()
{
	sf::RenderWindow window(sf::VideoMode(500, 500), "SFML works!", sf::Style::Fullscreen);
	sf::CircleShape shape(200.f);
	shape.setFillColor(sf::Color::Green);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::KeyReleased:
				switch (event.key.code)
				{
				case sf::Keyboard::Escape:
					window.create(sf::VideoMode(500, 500), "SFML works!", sf::Style::Default);
					break;
				case sf::Keyboard::Return:
					window.create(sf::VideoMode(500, 500), "SFML works!", sf::Style::Fullscreen);
					break;
				}
				break;
			}
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		window.draw(shape);
		window.display();
	}

	return 0;
}