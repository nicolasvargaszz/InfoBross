#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <optional>
#include <vector>

using namespace sf;

int main()
{
    // Window
    RenderWindow window(VideoMode({800, 600}), "SFML Platformer");
    window.setFramerateLimit(120);

    // Load textures
    Texture idleTexture, moveTexture1, moveTexture2;
    if (!idleTexture.loadFromFile("/Users/nicolasvargas/Desktop/Super-Mario-Bros-Main/Source/FotosMC/mathi.png") ||
        !moveTexture1.loadFromFile("/Users/nicolasvargas/Desktop/Super-Mario-Bros-Main/Source/FotosMC/mathi_movimiento.png") ||
        !moveTexture2.loadFromFile("/Users/nicolasvargas/Desktop/Super-Mario-Bros-Main/Source/FotosMC/mathi_movimiento_complemento.png"))
    {
        return -1;
    }

    // Player sprite
    Sprite player(idleTexture);
    {
        FloatRect b = player.getLocalBounds();
        player.setOrigin({ b.size.x / 2.f, b.size.y / 2.f });
    }
    player.setScale({ 0.5f, 0.5f });
    player.setPosition({ 400.f, 300.f });

    // Platforms
    std::vector<RectangleShape> platforms;
    auto makePlat = [&](float x, float y, float w, float h){
        RectangleShape r({ w, h });
        r.setPosition({ x, y });
        r.setFillColor(Color(100, 100, 200));
        platforms.push_back(r);
    };
    makePlat(  0, 580, 800, 20);
    makePlat(100, 450, 200, 20);
    makePlat(400, 350, 200, 20);
    makePlat(600, 500, 150, 20);

    // Physics constants
    const float GRAVITY  = 1200.f;  // px/s²
    const float MOVE_SPD =  300.f;  // px/s
    const float JUMP_SPD =  600.f;  // px/s

    Vector2f velocity{ 0.f, 0.f };
    bool    onGround = false;

    // Animation toggle
    bool  toggleMoveFrame = false;
    float toggleInterval  = 0.15f;
    Clock toggleClock;

    // Delta-time clock
    Clock dtClock;

    while (window.isOpen())
    {
        float dt = dtClock.restart().asSeconds();

        // --- EVENTS ---
        while (true)
        {
            std::optional<Event> e = window.pollEvent();
            if (!e.has_value()) break;
            if (e->is<Event::Closed>())
                window.close();
        }

        // --- INPUT ---
        bool isMoving = false;
        velocity.x = 0.f;

        if (Keyboard::isKeyPressed(Keyboard::Key::Left))
        {
            velocity.x = -MOVE_SPD;
            player.setScale({ -0.5f, 0.5f });  // face left
            isMoving = true;
        }
        else if (Keyboard::isKeyPressed(Keyboard::Key::Right))
        {
            velocity.x = MOVE_SPD;
            player.setScale({  0.5f, 0.5f });  // face right
            isMoving = true;
        }

        if (Keyboard::isKeyPressed(Keyboard::Key::Up) && onGround)
        {
            velocity.y = -JUMP_SPD;
            onGround = false;
        }

        // --- ANIMATION ---
        if (!isMoving)
        {
            toggleClock.restart();
            player.setTexture(idleTexture);
        }
        else
        {
            if (toggleClock.getElapsedTime().asSeconds() >= toggleInterval)
            {
                toggleMoveFrame = !toggleMoveFrame;
                toggleClock.restart();
            }
            player.setTexture(toggleMoveFrame ? moveTexture1 : moveTexture2);
        }

        // --- PHYSICS ---
        velocity.y += GRAVITY * dt;

        // Move X and handle collisions
        player.move({ velocity.x * dt, 0.f });
        {
            FloatRect pb = player.getGlobalBounds();
            for (auto &plat : platforms)
            {
                FloatRect pr = plat.getGlobalBounds();
                bool hitX =
                    pb.position.x + pb.size.x > pr.position.x &&
                    pb.position.x < pr.position.x + pr.size.x &&
                    pb.position.y + pb.size.y > pr.position.y &&
                    pb.position.y < pr.position.y + pr.size.y;

                if (hitX)
                {
                    if (velocity.x > 0.f)
                        player.setPosition({ pr.position.x - pb.size.x/2.f, player.getPosition().y });
                    else if (velocity.x < 0.f)
                        player.setPosition({ pr.position.x + pr.size.x + pb.size.x/2.f, player.getPosition().y });

                    velocity.x = 0.f;
                    pb = player.getGlobalBounds();
                }
            }
        }

        // Move Y and handle collisions
        player.move({ 0.f, velocity.y * dt });
        onGround = false;
        {
            FloatRect pb = player.getGlobalBounds();
            for (auto &plat : platforms)
            {
                FloatRect pr = plat.getGlobalBounds();
                bool hitY =
                    pb.position.x + pb.size.x > pr.position.x &&
                    pb.position.x < pr.position.x + pr.size.x &&
                    pb.position.y + pb.size.y > pr.position.y &&
                    pb.position.y < pr.position.y + pr.size.y;

                if (hitY)
                {
                    if (velocity.y > 0.f)
                    {
                        // land on top
                        player.setPosition({ player.getPosition().x, pr.position.y - pb.size.y/2.f });
                        onGround = true;
                    }
                    else if (velocity.y < 0.f)
                    {
                        // hit ceiling
                        player.setPosition({ player.getPosition().x, pr.position.y + pr.size.y + pb.size.y/2.f });
                    }
                    velocity.y = 0.f;
                    pb = player.getGlobalBounds();
                }
            }
        }

        // --- RENDER ---
        window.clear(Color(200, 200, 255));
        for (auto &plat : platforms)
            window.draw(plat);
        window.draw(player);
        window.display();
    }

    return 0;
}