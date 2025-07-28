#pragma once
#include <SFML/Graphics.hpp>

class ScoreLabel {
public:
	ScoreLabel(sf::Font& textFont);

	void draw(sf::RenderWindow& window);

	void setScore(int points);
	auto getScore() -> int;
	void updateScore(int points);

private:
	sf::Text scoreText;
	int score;
};