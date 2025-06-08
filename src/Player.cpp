#include "../include/Player.h"
#include "../include/TiledMap.h"  

Player::Player(const sf::Texture& texture)
    : sprite(texture), velocity(0.f, 0.f), onGround(false), facingRight(true), isMoving(false), currentAnimation(nullptr) {}

bool Player::loadAnimation(const std::string& name, const std::string& texturePath, int frameCount, float frameDuration) {
    sf::Texture texture;
    if (!texture.loadFromFile(texturePath))
        return false;

    textures[name] = texture;

    Animation anim(frameDuration);
    anim.setSpriteSheet(textures[name]); 

    sf::Vector2u texSize = textures[name].getSize();
    int frameWidth = texSize.x / frameCount;
    int frameHeight = texSize.y;

    for (int i = 0; i < frameCount; ++i) {
        sf::IntRect frameRect({i * frameWidth, 0}, {frameWidth, frameHeight});
        anim.addFrame(frameRect);
    }

    animations[name] = anim;

    if (!currentAnimation) {
        currentAnimation = &animations[name];
        sprite.setTexture(currentAnimation->getTexture());
        sprite.setTextureRect(currentAnimation->getCurrentFrame());

        sf::FloatRect bounds = sprite.getLocalBounds();
        sprite.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
    }

    return true;
}

void Player::setAnimation(const std::string& name) {
    auto it = animations.find(name);
    if (it != animations.end() && currentAnimation != &(it->second)) {
        currentAnimation = &(it->second);
        currentAnimation->reset();
        sprite.setTexture(currentAnimation->getTexture());
        sprite.setTextureRect(currentAnimation->getCurrentFrame());
    }
}

void Player::setPosition(const sf::Vector2f& pos) {
    sprite.setPosition(pos);
}

sf::Vector2f Player::getPosition() const {
    return sprite.getPosition();
}

void Player::handleInput() {
    isMoving = false;
    velocity.x = 0.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
        velocity.x = -MOVE_SPEED;
        isMoving = true;
        facingRight = false;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
        velocity.x = MOVE_SPEED;
        isMoving = true;
        facingRight = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && onGround) {
        velocity.y = -JUMP_SPEED;
        onGround = false;
    }
}

void Player::applyPhysics(float dt, const TiledMap& map) {
    velocity.y += GRAVITY * dt;

    sprite.move({velocity.x * dt, 0.f});
    if (map.isColliding(sprite.getGlobalBounds())) {
        sprite.move({-velocity.x * dt, 0.f});
    }

    float dy = velocity.y * dt;
    sprite.move({0.f, dy});

    if (map.isColliding(sprite.getGlobalBounds())) {
        sprite.move({0.f, -dy});
        if (velocity.y > 0.f) {
            onGround = true;
        } else {
            onGround = false;
        }
        velocity.y = 0.f;
    } else {
        onGround = false;
    }

    sf::FloatRect bounds = sprite.getGlobalBounds();
    sf::FloatRect feetArea({bounds.position.x, bounds.position.y + bounds.size.y - 5.f}, {bounds.size.x, 5.f});

    if (map.isColliding(feetArea)) {
        onGround = true;
    }
}


void Player::update(float dt) {
    if (!onGround) {
        // Mostrar solo el primer frame de la animación de salto (no avanzar)
        if (currentAnimation != &animations["jump"]) {
            setAnimation("jump");
            currentAnimation->reset();
        }
        sprite.setTextureRect(currentAnimation->getCurrentFrame());
    } else {
        if (isMoving) {
            setAnimation("walk");
        } else {
            setAnimation("idle");
        }
        // En estas animaciones sí actualizamos
        currentAnimation->update(dt);
        sprite.setTextureRect(currentAnimation->getCurrentFrame());
    }
    // Ajustar escala para mirar derecha o izquierda
    sprite.setScale({facingRight ? -SCALE : SCALE, SCALE});;
}

void Player::render(sf::RenderWindow& window) {
    window.draw(sprite);
}

sf::FloatRect Player::getBounds() const {
    return sprite.getGlobalBounds();
}