#include "alien.hpp"

Alien::Alien(sf::Texture& texture, sf::Vector2f position)
	: sprite(texture)
{
	sprite.setPosition(position);
}

void Alien::draw(sf::RenderWindow& window) {
	window.draw(sprite);
}

auto Alien::getAlienPosition() -> sf::Vector2f {
	return sprite.getPosition();
}
