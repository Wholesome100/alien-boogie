#include <SFML/Graphics.hpp>
#include <random>

class Alien {
public:
	Alien(sf::Texture& texture, sf::Vector2f position)
		: sprite(texture)
	{
		sprite.setPosition(position);
	}

	void draw(sf::RenderWindow& window) {
		window.draw(sprite);
	}

private:
	sf::Sprite sprite;
};