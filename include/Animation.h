#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Animation {
public:
    Animation(float frameTime = 0.1f);
    void setSpriteSheet(sf::Texture& tex);
    void addFrame(const sf::IntRect& rect);
    void update(float dt);
    const sf::Texture& getTexture() const;
    sf::IntRect getCurrentFrame() const;
    void reset();

private:
    std::vector<sf::IntRect> frames;
    float frameTime;
    float elapsedTime = 0.f;
    size_t currentFrame = 0;
    sf::Texture* texture = nullptr;
};
