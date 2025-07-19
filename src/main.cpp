#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <Windows.h>
#include <iostream>

#include "alien.hpp"
#include "audio_capture.hpp"

int main()
{
#pragma region WindowSetup
    const int SAFE_ZONE_PADDING{ 50 };
    int screen_x{ 800 };
    int screen_y{ 600 };

    
    auto modes{ sf::VideoMode::getFullscreenModes() };

#ifdef _DEBUG
    // Code block for printing supported fullscreen resolutions
    for (auto& mode : modes)
        std::cout << mode.size.x << " " << mode.size.y << std::endl;
#endif

    // Validate and assign largest window size
    // Sizes must be subtracted to prevent a borderless fullscreen window and create a "safe zone"
    if (!modes.empty()) {
        screen_x = modes[0].size.x - SAFE_ZONE_PADDING;
        screen_y = modes[0].size.y - SAFE_ZONE_PADDING;
    }

    sf::RenderWindow window(sf::VideoMode({ static_cast<unsigned int>(screen_x),  static_cast<unsigned int>(screen_y) }),
        "Boogie Test",
        sf::Style::None
        // Fullscreen state doesn't work. Best bet is to get the largest fullscreen size and set the videomode to that
    );
#pragma endregion

#pragma region TransparencySetup
    HWND hwnd{ window.getNativeHandle() };

    // Window transparency code for windows
    SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED | WS_EX_TRANSPARENT);
    
    // This is explicitly setting the background color to black
    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 0, LWA_COLORKEY);
#pragma endregion

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

        aliens.back().setState(AlienState::WALK);
        aliens.back().setTargetPosition(sf::Vector2f(
            static_cast<float>(std::rand() % window.getSize().x),
            static_cast<float>(std::rand() % window.getSize().y))
        );
    }

    const float ENERGY_THRESHOLD = 0.05f;
    AudioCapture audio;
    if (!audio.initialize()) {
        std::cerr << "Audio init failed\n";
        return -1;
    }
    audio.start();

    sf::Clock clock;
    while (window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        float energy = audio.calculateEnergy();
        std::cout << "Energy: " << energy << std::endl;

        // Used to wipe black pixels (our background) to make the window clear
        window.clear(sf::Color::Black);

        for (auto& alien : aliens) {
            if (energy > ENERGY_THRESHOLD) {
                std::cout << "Boogie Triggered! Energy = " << energy << std::endl;
                alien.setState(AlienState::BOOGIE);
            }
            else {
                alien.setState(AlienState::IDLE);
            }
            alien.update(deltaTime, window);
            alien.draw(window);
        }

        window.display();
    }
}