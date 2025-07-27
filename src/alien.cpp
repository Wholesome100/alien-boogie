#include <cmath>
#include <random>

#include "alien.hpp"

Alien::Alien(sf::Texture& texture, sf::Vector2f position)
	: sprite(texture)
{
	sprite.setPosition(position);
	sprite.setTextureRect(sf::IntRect({ 16, 0 }, { 16, 16 }));

	// Randomly assign a boogie type to the aliens
	thread_local std::mt19937 generator(std::random_device{}());
	std::uniform_int_distribution<int> dist(0, 2);
	switch (dist(generator)) {
	case 0: 
		boogieType = BoogieType::ONE; 
		break;
	case 1: 
		boogieType = BoogieType::TWO; 
		break;
	case 2: 
		boogieType = BoogieType::THREE; 
		break;
	}
}

void Alien::draw(sf::RenderWindow& window) {
	window.draw(sprite);
}

// update alters the position and animation states of the alien every frame
void Alien::update(float delta, const sf::RenderWindow& window) {
	// Code block to calculate alien movement
	// While our aliens are walking, make them lerp to their new position
	if (moveState == MovementState::WALK) {
		sf::Vector2f current{ sprite.getPosition() };
		sf::Vector2f direction{ targetPosition - current };

		// Get the bool for if we're facing left or right
		faceRight = (direction.x >= 0);
		
		//Calculate euclidean distance and assign to length
		float length{ std::sqrt((direction.x * direction.x) + (direction.y * direction.y)) };
		if (length > 1.0f) {
			direction /= length;
			sprite.move(direction * SPEED * delta);
		}
		else {
			// When an alien reaches its target, set them back to IDLE
			moveState = MovementState::IDLE;
		}
	}

	// Track time for animations
	elapsedTime += delta;

	// Code block to determine animation priority
	// Zapped block breaks the rules a bit by moving aliens while animating
	if (actionState == ActionState::ZAPPED) {
		// Zapped aliens will be set to the last frame
		sprite.setTextureRect(sf::IntRect{ { 96, 0 }, { 16, 16 } });

		sprite.move({0.0f, ZAP_FALLSPEED * delta});
		if (sprite.getPosition().y > window.getSize().y) {
			// When an alien goes offscreen, set its state to DEAD
			setActionState(ActionState::NONE);
			setMovementState(MovementState::DEAD);
		}
	}
	else if (actionState == ActionState::BOOGIE) {
		boogieDuration -= delta;
		switch (boogieType) {
		case BoogieType::ONE: 
			animate(delta, BOOGIE_ONE); 
			break;
		case BoogieType::TWO: 
			animate(delta, BOOGIE_TWO); 
			break;
		case BoogieType::THREE: 
			animate(delta, BOOGIE_THREE); 
			break;
		}

		if (boogieDuration < 0.0f)
			setActionState(ActionState::NONE);
	}
	else if (moveState == MovementState::WALK) {
		// Play the walk animation
		faceRight ? animate(delta, RIGHT_WALK) : animate(delta, LEFT_WALK);

	}
	else {
		// Play the idle animation
		animate(delta, IDLE_FRAMES);
	}

}


void Alien::animate(float delta, const std::vector<sf::IntRect>& spriteFrames) {
	if (elapsedTime >= FRAME_TIME) {
		elapsedTime = 0.0f;

		sprite.setTextureRect(
			spriteFrames[++currentFrame % spriteFrames.size()]
		);
	}
}

// A function that has a chance to be called during update, will make aliens walk to a new position
void Alien::goWalk(const sf::Vector2f& target) {
	moveState = MovementState::WALK;
	targetPosition = target;
}

void Alien::respawn(const sf::Vector2f& newSpawn) {
	moveState = MovementState::IDLE;
	sprite.setPosition(newSpawn);
}

#pragma region Accessors + Mutators
void Alien::setMovementState(MovementState newState){
	moveState = newState;
}

auto Alien::getMovementState() -> MovementState{
	return moveState;
}


void Alien::setActionState(ActionState newState) {
	actionState = newState;
}

auto Alien::getActionState() -> ActionState {
	return actionState;
}

void Alien::setBoogieDuration(float boogieTime) {
	boogieDuration = boogieTime;
}

auto Alien::getBounds() -> sf::FloatRect {
	return sprite.getGlobalBounds();
}
#pragma endregion