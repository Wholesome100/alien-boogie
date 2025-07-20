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

// update alters the position and animation states of the alien every frame
void Alien::update(float delta, const sf::RenderWindow& window) {
	// While our aliens are walking, make them lerp to their new position
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

	// Code block to determine animation priority
	if (actionState != ActionState::NONE) {
		if (actionState == ActionState::ZAPPED) {
			// Play the animation for being zapped
		}
		else if (actionState == ActionState::BOOGIE) {
			// Play the animation for the boogie
		}
	}
	else if (moveState == MovementState::WALK) {
		// Play the walk animation
	}
	else {
		// Play the idle animation
	}

}

// A function that has a chance to be called during update, will make aliens walk to a new position
void Alien::goWalk(float delta) {
	std::ignore = delta;
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