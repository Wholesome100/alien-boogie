#pragma once
#include <SFML/Graphics.hpp>

enum class AlienState {
    IDLE,
    WALK,
    BOOGIE
};

class Alien {
public:
    Alien(sf::Texture& texture, sf::Vector2f position);
   
    void draw(sf::RenderWindow& window);
    void update(float delta, const sf::RenderWindow& window);

    void setState(AlienState newState);
    auto getState() -> AlienState;

    void setTargetPosition(const sf::Vector2f& target);
    auto getTargetPosition() -> sf::Vector2f;

private:
    sf::Sprite sprite;
    AlienState state{ AlienState::IDLE };

    sf::Vector2f targetPosition;
    static constexpr float SPEED = 100.f;
};