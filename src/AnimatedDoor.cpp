#include "../include/AnimatedDoor.h"

AnimatedDoor::AnimatedDoor() : sprite(std::nullopt), animation(nullptr), playing(false) {}

void AnimatedDoor::setTexture(const sf::Texture& tex) {
    sprite.emplace(tex); 
}

void AnimatedDoor::setAnimation(Animation* anim) {
    animation = anim;
    if (animation) {
        if (!sprite.has_value())
            sprite.emplace(animation->getTexture());
        else
            sprite->setTexture(animation->getTexture());

        sprite->setTextureRect(animation->getCurrentFrame());
    }
}

void AnimatedDoor::setPosition(const sf::Vector2f& pos) {
    if (sprite)
        sprite->setPosition(pos);
}

void AnimatedDoor::update(float dt) {
    if (!animation || !playing || !sprite) return;

    animation->update(dt);
    sprite->setTextureRect(animation->getCurrentFrame());
}

void AnimatedDoor::play() {
    playing = true;
    if (animation)
        animation->reset();
}

void AnimatedDoor::stop() {
    playing = false;
}

bool AnimatedDoor::isPlaying() const {
    return playing;
}

void AnimatedDoor::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (sprite)
        target.draw(*sprite, states);  
}
