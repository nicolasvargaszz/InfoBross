//// filepath: /Users/nicolasvargas/Desktop/Super-Mario-Bros-Main/src/cafe.cpp
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
    cafes.emplace_back(sf::Vector2f(60.f, 500.f));
    //cafes.emplace_back(sf::Vector2f(70.f, 400.f));
    //cafes.emplace_back(sf::Vector2f(80.f, 300.f));
    //cafes.emplace_back(sf::Vector2f(65.f, 290.f));
    // Add as many as you want—
    //cafes.emplace_back(sf::Vector2f(90.f, 500.f));
    //cafes.emplace_back(sf::Vector2f(120.f, 480.f));
    cafes.emplace_back(sf::Vector2f(140.f, 480.f));
    //cafes.emplace_back(sf::Vector2f(160.f, 480.f));
    //cafes.emplace_back(sf::Vector2f(180.f, 480.f));
    //.emplace_back(sf::Vector2f(200.f, 480.f));

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
    auto intersection = playerBounds.findIntersection(sprite.getGlobalBounds());
    if (intersection.has_value())
    {
        // If the player collides, increment the count and move this café off-screen
        cafeCounter++;
        sprite.setPosition({-1000.f, -1000.f});

        // If we want you to finish after collecting 5 cafés from this approach:
        if (cafeCounter >= 5)
        {
            gameFinished = true;
        }
    }
}

void Cafe::render(sf::RenderWindow& window)
{
    window.draw(sprite);
}

sf::FloatRect Cafe::getBounds() const
{
    return sprite.getGlobalBounds();
}