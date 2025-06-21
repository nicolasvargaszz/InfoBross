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

// 1-) New static Texture for the 4 separates frames

static sf::Texture sprite_1;
static sf::Texture sprite_2;
static sf::Texture sprite_3;
static sf::Texture sprite_4;


Enemy::Enemy(sf::Texture& texture, const sf::Vector2f& startPos)
: sprite(texture)
{
    if (!sprite_1.loadFromFile("../assets/sprites/enemy/sprite_1.png")) {
        std::cerr << "Error: No se pudo cargar sprite_2.png\n";
    }
    if (!sprite_2.loadFromFile("../assets/sprites/enemy/sprite_2.png")) {
        std::cerr << "Error: No se pudo cargar sprite_2.png\n";
    }
    if (!sprite_3.loadFromFile("../assets/sprites/enemy/sprite_3.png")) {
        std::cerr << "Error: No se pudo cargar sprite_2.png\n";
    }
    if (!sprite_4.loadFromFile("../assets/sprites/enemy/sprite_4.png")) {
        std::cerr << "Error: No se pudo cargar sprite_2.png\n";
    }
    
    //state with the first texture
    sprite.setTexture(sprite_1);

    sprite.setPosition(startPos);
    sprite.setScale({1.f, 1.f}); // if we change this the size of the enemy will change
    // general enemy properties
    speed      = 10.f;
    direction  = 1.f; // 1 = right, -1 = left
    velocityY  = 0.f;
    onGround   = false;

    animationTime = 0.1f; // time for cycling the frames
    currentFrame = 0; // start with the first frame
}

void Enemy::update(float deltaTime, TiledMap* map, Player* player)
{
    // step no 3 for the enemy animation:
    animationTime += deltaTime;
    if(animationTime >= 0.15f)
    {
        animationTime = 0.f;
        currentFrame = (currentFrame + 1) % 4; // Cycle through
        switch(currentFrame)
        {
            case 0: sprite.setTexture(sprite_1); break;
            case 1: sprite.setTexture(sprite_2); break;
            case 2: sprite.setTexture(sprite_3); break;
            case 3: sprite.setTexture(sprite_4); break;
        }
    }

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
                // player->setPosition({-9999.f, -9999.f}); uncomment this line to move player off-screen when is killed by the enemy, i just commented becuase when getting kill every time i want to test the code xDDDDD
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