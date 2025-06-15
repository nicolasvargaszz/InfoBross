#pragma once
#include <SFML/Graphics.hpp>

class FadeTransition {
public:
    enum class State { None, FadingOut, FadingIn, Done };

    FadeTransition(sf::Vector2u windowSize, sf::Time duration);

    void start();
    void update();
    void draw(sf::RenderWindow& window);

    bool isRunning() const;
    State getState() const;

private:
    sf::RectangleShape fadeRect;
    sf::Clock clock;
    sf::Time totalDuration;
    sf::Time halfDuration;
    State currentState;
};
