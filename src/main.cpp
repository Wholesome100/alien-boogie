#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

// Omitting header files for simplicity
#include "alien.cpp"
#include "audio_manager.cpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 1920, 1080 }), 
        "Boogie Test",
        sf::Style::None
    );

    //sf::CircleShape shape(100.f);
    //shape.setFillColor(sf::Color::Green);

    // Need to see how to properly bundle assets with SFML
    sf::Texture texture;
    if (!texture.loadFromFile("../../../../src/assets/alien-base.png"))
        return -1;

    std::vector<Alien> aliens;
    const int NUM_ALIENS = 100;

    for (int i = 0; i < NUM_ALIENS; ++i) {
        float x = std::rand() % window.getSize().x;
        float y = std::rand() % window.getSize().y;
        aliens.emplace_back(texture, sf::Vector2f(x, y));
    }

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();
        //window.draw(shape);
        for (auto& alien : aliens)
            alien.draw(window);

        window.display();
    }
}