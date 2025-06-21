#include "../include/cafe.h"
#include <iostream>

// One shared texture so multiple Cafe instances can reuse it.
static sf::Texture s_cafeTexture;

// Helper function: returns a vector with all your Café positions.
std::vector<Cafe> createCafes()
{
    // Load the texture once (if not already loaded)
    if (!s_cafeTexture.loadFromFile("../assets/sprites/CAFE.png"))
    {
        std::cerr << "Error loading the cafe texture\n";
    }

    // Create multiple Café objects.
    // Each Cafe uses the static texture above through the constructor.
    std::vector<Cafe> cafes;
    cafes.emplace_back(sf::Vector2f(140.f, 480.f));


    return cafes;
}

// -----------------------------------------------------

Cafe::Cafe(const sf::Vector2f& position)
: sprite(s_cafeTexture) // <-- Initialize the sprite with the shared texture
{
    sprite.setPosition(position);
    sprite.setScale({2.5f, 2.5f});
}

void Cafe::update(sf::FloatRect playerBounds, int& cafeCounter, bool& gameFinished)
{
    if (collected) return;
    auto intersection = playerBounds.findIntersection(sprite.getGlobalBounds());
    if (intersection.has_value())
    {
         collected = true;
        // If the player collides, increment the count and move this café off-screen
        cafeCounter++;
        sprite.setPosition({-1000.f, -1000.f});

        // If we want you to finish after collecting 5 cafés from this approach:
        if (cafeCounter >= 10)
        {
            gameFinished = true;
        }
    }
}

void Cafe::render(sf::RenderWindow& window)
{
    if (!collected)
        window.draw(sprite);
}

sf::FloatRect Cafe::getBounds() const
{
    return sprite.getGlobalBounds();
}
