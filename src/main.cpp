#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <Windows.h>

#include "alien.hpp"
#include "audio_manager.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 1920, 1080 }), 
        "Boogie Test",
        sf::Style::None
    );

    
    HWND hwnd = window.getNativeHandle();

    // Window transparency code for windows
    SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED | WS_EX_TRANSPARENT);
    
    // This is explicitly setting the background color to black
    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 0, LWA_COLORKEY);

    // Need to see how to properly bundle assets with SFML
    sf::Texture texture;
    if (!texture.loadFromFile("assets/alien-base.png"))
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

        // Used to wipe black pixels (our background) to make the window clear
        window.clear(sf::Color::Black);
        window.clear();
    
        for (auto& alien : aliens)
            alien.draw(window);

        window.display();
    }
}