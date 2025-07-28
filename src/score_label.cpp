#include "score_label.hpp"

ScoreLabel::ScoreLabel(sf::Font& textFont)
	: scoreText{ textFont },
	score{ 0 }
{
	scoreText.setString("Aliens Zapped:" + std::to_string(score));
	scoreText.setCharacterSize(100.f);
	scoreText.setFillColor(sf::Color::Green);

	scoreText.setOutlineColor(sf::Color::White);
	scoreText.setOutlineThickness(2.f);
}

// set the text to the new score and then draw the label
void ScoreLabel::draw(sf::RenderWindow& window) {
	scoreText.setString("Aliens Zapped: " + std::to_string(score));
	window.draw(scoreText);
}

void ScoreLabel::setScore(int points) {
	score = points;
}

int ScoreLabel::getScore() {
	return score;
}

// Function to add points to the current value
void ScoreLabel::updateScore(int points) {
	score += points;
}