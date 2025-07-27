#pragma once
#include <SFML/Graphics.hpp>

enum class MovementState {
    IDLE,
    WALK,
    DEAD
};

enum class ActionState {
    NONE,
    BOOGIE,
    ZAPPED,
};

enum class BoogieType {
    ONE,
    TWO,
    THREE
};

class Alien {
public:
    Alien(sf::Texture& texture, sf::Vector2f position);
   
    void draw(sf::RenderWindow& window);
    void update(float delta, const sf::RenderWindow& window);
    void animate(float delta, const std::vector<sf::IntRect>& spriteFrames);

    void goWalk(const sf::Vector2f& target);
    void respawn(const sf::Vector2f& newSpawn);

    void setMovementState(MovementState newState);
    auto getMovementState() -> MovementState;

    void setActionState(ActionState newState);
    auto getActionState() -> ActionState;

    auto getBounds() -> sf::FloatRect;

private:
    sf::Sprite sprite;

    // Var to give each alien a unique dance style
    BoogieType boogieType = BoogieType::ONE;

    // Variables to store the frames for certain animations
    const std::vector<sf::IntRect> 
        IDLE_FRAMES
    {
        sf::IntRect{{ 0, 0 }, { 16, 16 }},
        sf::IntRect{{ 16, 0 }, { 16, 16 }}
    };

    const std::vector<sf::IntRect>
        LEFT_WALK
    {
        sf::IntRect{{ 32, 0 }, { 16, 16 }},
        sf::IntRect{{ 48, 0 }, { 16, 16 }}
    };

    // To flip a frame, we need to go to the end of the frame, and then use a negative width
    const std::vector<sf::IntRect>
        RIGHT_WALK
    {
        sf::IntRect{{ 32 + 16, 0 }, { -16, 16 }},
        sf::IntRect{{ 48 + 16 , 0 }, { -16, 16 }}
    };

    // Same flip logic here
    const std::vector<sf::IntRect>
        BOOGIE_ONE
    {
        sf::IntRect{{ 64, 0 }, { 16, 16 }},
        sf::IntRect{{ 64 + 16, 0 }, { -16, 16 }}
    };

    const std::vector<sf::IntRect>
        BOOGIE_TWO
    {
        sf::IntRect{{ 80, 0 }, { 16, 16 }},
        sf::IntRect{{ 96, 0 }, { 16, 16 }}
    };

    const std::vector<sf::IntRect>
        BOOGIE_THREE
    {
        sf::IntRect{{ 0, 0 }, { 16, 16 }},
        sf::IntRect{{ 80, 0 }, { 16, 16 }}
    };

    MovementState moveState{ MovementState::IDLE };
    ActionState actionState{ ActionState::NONE };

    sf::Vector2f targetPosition;
    static constexpr float SPEED = 100.f;
    static constexpr float ZAP_FALLSPEED = 240.f;

    int currentFrame = 0;
    float elapsedTime = 0.0f;
    const float FRAME_TIME = 0.2f;
};