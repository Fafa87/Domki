#include <SFML/Graphics.hpp>

int main()
{
	sf::RenderWindow window(sf::VideoMode(1600, 800), "SFML works!", sf::Style::Default);
	sf::CircleShape shape(200.f);
	shape.setFillColor(sf::Color::Green);
	shape.setOrigin(200, 200);
	shape.setPosition(500, 0);

	sf::CircleShape shape2(300.f);
	shape2.setFillColor(sf::Color::Red);

	sf::Texture texture;
	if (!texture.loadFromFile("C:\\Users\\Szpieg\\Desktop\\SuperDomki\\Domki-master\\Grafika\\znaczek.bmp"))
	{
		int dukpa = 3;
		// error...
	}


	sf::Texture backtexture;
	if (!backtexture.loadFromFile("C:\\Users\\Szpieg\\Desktop\\SuperDomki\\Domki-master\\Grafika\\kafle.jpg"))
	{

	}
	backtexture.setRepeated(true);
	sf::Sprite background(backtexture);
	background.setTextureRect({ 0, 0, 1600, 800 });

	shape.setTexture(&texture); // texture is a sf::Texture
	shape.setTextureRect(sf::IntRect(10, 10, 100, 100));

	double speed = 0.1;
	double speedAngle = 0.1;
	while (window.isOpen())
	{
		sf::Event event;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
		{
			shape.setRotation(shape.getRotation() - speedAngle);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
		{
			shape.setRotation(shape.getRotation() + speedAngle);
		}
		else {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				shape.setPosition(shape.getPosition().x - speed, shape.getPosition().y);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				shape.setPosition(shape.getPosition().x, shape.getPosition().y + speed);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				shape.setPosition(shape.getPosition().x + speed, shape.getPosition().y);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				shape.setPosition(shape.getPosition().x, shape.getPosition().y - speed);
			}
		}

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
		window.draw(background);
		window.draw(shape2);
		window.draw(shape);
		window.display();
	}

	return 0;
}