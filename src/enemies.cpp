//// filepath: /Users/nicolasvargas/Desktop/Super-Mario-Bros-Main/src/enemies.cpp
#include "../include/Enemy.h"
#include "../include/TiledMap.h"  // only if you need TiledMap methods
#include <iostream>

Enemy::Enemy(sf::Texture& texture, const sf::Vector2f& startPos)
: sprite(texture)
{
    sprite.setPosition(startPos);
    sprite.setScale({1.f, 1.f});

    speed      = 50.f;
    direction  = 1.f; // 1 = right, -1 = left
    velocityY  = 0.f;
    onGround   = false;
}

void Enemy::update(float deltaTime, TiledMap* map)
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
        // Revertir el movimiento en Y
        sprite.move({0.f, -moveY});

        // Si el enemigo se movía hacia abajo, significa que pisó algo
        if (velocityY > 0.f)
        {
            onGround = true;
        }
        else
        {
            // Choca desde abajo
        }
        // Detener velocidad vertical al chocar
        velocityY = 0.f;
    }
    else
    {
        onGround = false;
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