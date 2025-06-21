#include "../include/Cafe.h"
#include <iostream>


bool Cafe::intersects(const sf::FloatRect& a, const sf::FloatRect& b) {
    return (a.position.x < b.position.x + b.size.x &&
            a.position.x + a.size.x > b.position.x &&
            a.position.y < b.position.y + b.size.y &&
            a.position.y + a.size.y > b.position.y);
}

Cafe::Cafe(sf::Texture& texture, const sf::Vector2f& startPos)
: sprite(texture), originalPos(startPos)
{
    sprite.setPosition(startPos);
    sprite.setScale({1.0f, 1.0f});
}

void Cafe::update(const sf::FloatRect& playerBounds, int& cafeCounter, bool& gameFinished) {
    if (collected) return;

    // Floating effect
    const float speed = 0.2f;
    const float maxOffset = 5.f;

    floatOffset += (goingUp ? -speed : speed);
    if (std::abs(floatOffset) >= maxOffset) {
        goingUp = !goingUp;
    }

    sprite.setPosition({originalPos.x, originalPos.y + floatOffset});   
    
    // Check for collection
    if (intersects(sprite.getGlobalBounds(), playerBounds)) {
        collected = true;
        cafeCounter++;

        // optional: hide it
        sprite.setPosition({-9999.f, -9999.f});

        // win condition at 10 cafés
        if (cafeCounter >= 10) {
            gameFinished = true;
        }
    }

}

void Cafe::render(sf::RenderWindow& window) const {
    if (!collected)
        window.draw(sprite);
}

sf::FloatRect Cafe::getBounds() const {
    return sprite.getGlobalBounds();
}


