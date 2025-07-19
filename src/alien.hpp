#pragma once
#include <SFML/Graphics.hpp>

class Alien {
public:
    Alien(sf::Texture& texture, sf::Vector2f position);

    enum class AlienState {
        IDLE,
        WALK,
        BOOGIE
    };

    void draw(sf::RenderWindow& window);
    auto getAlienPosition() -> sf::Vector2f;

private:
    sf::Sprite sprite;
};