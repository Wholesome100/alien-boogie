#pragma once
#include <SFML/Graphics.hpp>

enum class MovementState {
    IDLE,
    WALK
};

enum class ActionState {
    NONE,
    BOOGIE,
    ZAPPED
};

class Alien {
public:
    Alien(sf::Texture& texture, sf::Vector2f position);
   
    void draw(sf::RenderWindow& window);
    void update(float delta, const sf::RenderWindow& window);

    void goWalk(float delta);

    void setMovementState(MovementState newState);
    auto getMovementState() -> MovementState;

    void setTargetPosition(const sf::Vector2f& target);
    auto getTargetPosition() -> sf::Vector2f;

    auto getBounds() -> sf::FloatRect;

private:
    sf::Sprite sprite;
    MovementState moveState{ MovementState::IDLE };
    ActionState actionState{ ActionState::NONE };

    sf::Vector2f targetPosition;
    static constexpr float SPEED = 100.f;

    float bounceTimer = 0.0f;
    const float BOUNCE_SPEED = 6.0f;
    const float BOUNCE_HEIGHT = 10.0f;
};