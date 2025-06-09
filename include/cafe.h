#pragma once
#include <SFML/Graphics.hpp>
#include<vector>
class Cafe {
public:
    Cafe(const sf::Vector2f& position);
    
    void update(sf::FloatRect playerBounds, int& cafeCounter, bool& gameFinished);
    void render(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;

private:
    sf::Sprite sprite;
    sf::Texture texture;
};

std::vector<Cafe> createCafes(); // Function to create multiple Cafe objects