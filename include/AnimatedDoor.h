#pragma once
#include <SFML/Graphics.hpp>
#include "Animation.h"
#include <optional>

class AnimatedDoor : public sf::Drawable {
public:
    AnimatedDoor();
    void setAnimation(Animation* anim);
    void setPosition(const sf::Vector2f& pos);
    void update(float dt);
    void play();
    void stop();
    bool isPlaying() const;
    void setTexture(const sf::Texture& tex);

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    Animation* animation;
    std::optional<sf::Sprite> sprite;
    bool playing;
};
