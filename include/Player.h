#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include "Animation.h"
#include "TiledMap.h"

class Player {
public:
    Player(const sf::Texture& texture);

    bool loadAnimation(const std::string& name, const std::string& texturePath, int frameCount, float frameDuration);
    void setAnimation(const std::string& name);

    void setPosition(const sf::Vector2f& pos);
    sf::Vector2f getPosition() const;
    sf::FloatRect getBounds() const;

    void handleInput();
    void applyPhysics(float dt, const TiledMap& map);
    void update(float dt);
    void render(sf::RenderWindow& window);

private:
    sf::Sprite sprite;
    sf::Vector2f velocity;
    bool onGround;
    bool facingRight;
    bool isMoving;

    std::unordered_map<std::string, Animation> animations;
    std::unordered_map<std::string, sf::Texture> textures; // ← evitar textura local destruida
    Animation* currentAnimation;

    // Constantes
    const float MOVE_SPEED = 120.f;
    const float JUMP_SPEED = 250.f;
    const float GRAVITY = 500.f;
    const float SCALE = 1.75f;
};
