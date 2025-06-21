#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class ExitButton {
public:
    ExitButton(sf::RenderWindow& window, const sf::Font& font);

    void handleEvent(const sf::Event& event);
    void update();
    void render();

    bool isClicked() const;

private:
    sf::RenderWindow& window;
    sf::RectangleShape button; 
    sf::Text buttonText;
    const sf::Font& font;
    bool clicked;
};
