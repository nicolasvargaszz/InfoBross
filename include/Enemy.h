#pragma once
#include <SFML/Graphics.hpp>

// 1) Forward declaration para evitar ciclo de includes:
class TiledMap;
class Player;

class Enemy {
public:
    Enemy(sf::Texture& texture, const sf::Vector2f& startPos);

    // update pasa un puntero TiledMap (ahora sí compilará)
    void update(float deltaTime, TiledMap* map, Player* player);

    sf::FloatRect getBounds() const;

    void render(sf::RenderWindow& window);
    

    void onPlayerCollision(bool fromAbove);

private:
    sf::Sprite sprite;
    float speed;
    float direction;
    float velocityY;
    bool onGround;
};