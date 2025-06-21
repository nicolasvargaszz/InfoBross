#pragma once
#include <SFML/Graphics.hpp>

class Cafe {
public:
    // Construct with a shared texture and a world position
    Cafe(sf::Texture& texture, const sf::Vector2f& startPos);

    // Called each frame to detect collection
    //   playerBounds: player's AABB
    //   cafeCounter: increments when collected
    //   gameFinished: set true if threshold reached
    void update(const sf::FloatRect& playerBounds, int& cafeCounter, bool& gameFinished);

    // Draw only if not yet collected
    void render(sf::RenderWindow& window) const;

    // Expose for optional collision queries
    sf::FloatRect getBounds() const;

private:
    sf::Sprite sprite;
    bool collected = false;

    // Simple AABB intersection helper
    static bool intersects(const sf::FloatRect& a, const sf::FloatRect& b);
};
