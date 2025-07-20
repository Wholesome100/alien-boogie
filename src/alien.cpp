#include "alien.hpp"
#include <cmath>

Alien::Alien(sf::Texture& texture, sf::Vector2f position)
	: sprite(texture)
{
	sprite.setPosition(position);
}

void Alien::draw(sf::RenderWindow& window) {
	window.draw(sprite);
}

void Alien::update(float delta, const sf::RenderWindow& window) {
	if (moveState == MovementState::WALK) {
		sf::Vector2f current{ sprite.getPosition() };
		sf::Vector2f direction{ targetPosition - current };

		//Calculate euclidean distance and assign to length
		float length{ std::sqrt((direction.x * direction.x) + (direction.y * direction.y)) };
		if (length > 1.0f) {
			direction /= length;
			sprite.move(direction * SPEED * delta);
		}
		else {
			moveState = MovementState::IDLE;
		}
	}
	/*else if (state == AnimationState::BOOGIE) {
		bounceTimer += delta;

		float offsetY = std::sin(bounceTimer * BOUNCE_SPEED) * BOUNCE_HEIGHT;

		sf::Vector2f basePos = sprite.getPosition();
		sprite.setPosition({ basePos.x, basePos.y + offsetY });
	}*/
}


void Alien::setMovementState(MovementState newState){
	moveState = newState;
}

auto Alien::getMovementState() -> MovementState{
	return moveState;
}


void Alien::setTargetPosition(const sf::Vector2f& target) {
	targetPosition = target;
}

auto Alien::getTargetPosition() -> sf::Vector2f {
	return targetPosition;
}


auto Alien::getBounds() -> sf::FloatRect {
	return sprite.getGlobalBounds();
}