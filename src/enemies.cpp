
#include "../include/Enemy.h"
#include "../include/TiledMap.h"  // only if you need TiledMap methods
#include "../include/Player.h"
#include <iostream>

//definition of intersects function

namespace
{
    //procide a local intersects function

    bool intersects(const sf::FloatRect& a, const sf::FloatRect& b)
    {
        return (a.position.x < b.position.x + b.size.x &&
                a.position.x + a.size.x > b.position.x &&
                a.position.y < b.position.y + b.size.y &&
                a.position.y + a.size.y > b.position.y);
    }
}

Enemy::Enemy(sf::Texture& texture, const sf::Vector2f& startPos)
: sprite(texture)
{
    sprite.setPosition(startPos);
    sprite.setScale({1.f, 1.f});

    speed      = 10.f;
    direction  = 1.f; // 1 = right, -1 = left
    velocityY  = 0.f;
    onGround   = false;
}

void Enemy::update(float deltaTime, TiledMap* map, Player* player)
{
    // 1. Aplicar gravedad si no está en el suelo
    const float gravity = 400.f;
    if(!onGround)
        velocityY += gravity * deltaTime;

    //------------------------------------------------------
    // 2. Mover en X y checar colisión con el mapa
    //------------------------------------------------------
    float moveX = speed * direction * deltaTime;
    sprite.move({moveX, 0.f});

    if (map && map->isColliding(sprite.getGlobalBounds()))
    {
        // Revertir el movimiento en X
        sprite.move({-moveX, 0.f});

        // Opcional: invertir dirección para “rebotar”
        direction = -direction;
        if (direction > 0.f)
        {
            sprite.setScale({-1.f, 1.f});
        }
        else
        {
            sprite.setScale({1.f, 1.f});
        }
    }

    //------------------------------------------------------
    // 3. Mover en Y y checar colisión con el mapa
    //------------------------------------------------------
    float moveY = velocityY * deltaTime;
    sprite.move({0.f, moveY});

    if (map && map->isColliding(sprite.getGlobalBounds()))
    {
        sprite.move({0.f, -moveY}); // Revertir movimiento en Y
        if (velocityY > 0.f)
        {
            onGround = true; // Colisión al caer
        }
        velocityY = 0.f; // Resetear velocidad vertical al tocar el suelo
    }
    else
    {
        onGround = false; // No está en el suelo
    }

    if(player)
    {
        sf::FloatRect eBounds = getBounds();
        sf::FloatRect pBounds = player->getBounds();

        // Check collision with player
        if (intersects(eBounds, pBounds))
        {
            float playerBottom = pBounds.position.y + pBounds.size.y;
            float enemyTop     = eBounds.position.y + 5.f;
            bool fromAbove     = (playerBottom < enemyTop);

            if (fromAbove)
            {
                // Kill enemy
                sprite.setPosition({-9999.f, -9999.f});
                std::cout << "Enemy killed by player!\n";
            }
            else
            {
                // Kill player
                player->setPosition({-9999.f, -9999.f});
                std::cout << "Player killed by enemy!\n";
            }
        }
    }
}


void Enemy::render(sf::RenderWindow& window)
{
    window.draw(sprite);
}

sf::FloatRect Enemy::getBounds() const
{
    return sprite.getGlobalBounds();
}

void Enemy::onPlayerCollision(bool fromAbove)
{
    if (fromAbove)
    {
        // “Kill” enemy by moving it off-screen
        sprite.setPosition({-9999.f, -9999.f});
    }
    else
    {
        std::cout << "Player killed by enemy!" << std::endl;
        // Example: reduce player's life
    }
}