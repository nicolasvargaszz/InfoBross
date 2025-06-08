#include "../include/Animation.h"

Animation::Animation(float frameTime)
    : frameTime(frameTime), elapsedTime(0.f), currentFrame(0), texture(nullptr) {}

void Animation::setSpriteSheet(sf::Texture& tex) {
    texture = &tex;
}

void Animation::addFrame(const sf::IntRect& rect) {
    frames.push_back(rect);
}

void Animation::update(float dt) {
    if (frames.empty())
        return;

    elapsedTime += dt;
    if (elapsedTime >= frameTime) {
        elapsedTime = 0.f;
        currentFrame = (currentFrame + 1) % frames.size();
    }
}

const sf::Texture& Animation::getTexture() const {
    if (!texture) throw std::runtime_error("Animation: Texture not set");
    return *texture;
}

sf::IntRect Animation::getCurrentFrame() const {
    if (frames.empty()) 
        return sf::IntRect();  // retorna un rect vacío si no hay frames
    return frames[currentFrame];
}

void Animation::reset() {
    currentFrame = 0;
    elapsedTime = 0.f;
}