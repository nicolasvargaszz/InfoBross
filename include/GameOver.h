#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Dialogue.h"

class GameOverScreen {
public:
    GameOverScreen(const sf::Font& font, const std::vector<std::string>& lines, sf::Vector2u windowSize);

    void update(float dt);
    void handleEvent(const sf::Event& ev);
    void draw(sf::RenderWindow& window);

    bool isFinished() const;
    void reset();

    bool isRetryClicked(const sf::RenderWindow& window, const sf::Event& ev) const;

private:
    Dialogue dialogue;
    sf::Text retryButton;
    bool finished;
};