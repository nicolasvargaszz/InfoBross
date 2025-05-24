#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <optional>
#include <vector>

using namespace sf;

int main()
{
    // 1) Correctly construct the window with SFML 3.0.1
    // Create VideoMode object separately, passing size as sf::Vector2u
    VideoMode mode({1600, 900}); // Corrected constructor
    RenderWindow window(mode, "INFOBROS");
    window.setFramerateLimit(60);

    // 2) Use openFromFile(...) for Font in SFML 3.0.1
    Font font;
    if (!font.openFromFile("/Users/nicolasvargas/Desktop/Super-Mario-Bros-Main/Source/FotosMC/GAME_glm.ttf"))
        return -1;

    // 3) For SFML 3.0.1, Text(const Font&, String, unsigned int)
    Text title(font, "INFOBROS", 128U);
    {
        FloatRect tb = title.getLocalBounds();
        // Use .size.x and .size.y for sf::FloatRect dimensions
        title.setOrigin({ tb.size.x * 0.5f, tb.size.y * 0.5f });
        title.setPosition({ window.getSize().x * 0.5f, window.getSize().y * 0.5f });
        title.setFillColor(Color::Yellow);
    }

    Text prompt(font, "Press any key to start", 32U);
    {
        FloatRect pb_prompt_bounds = prompt.getLocalBounds();
        // Use .size.x and .size.y for sf::FloatRect dimensions
        prompt.setOrigin({ pb_prompt_bounds.size.x * 0.5f, pb_prompt_bounds.size.y * 0.5f });
        prompt.setPosition({ window.getSize().x * 0.5f, window.getSize().y * 0.7f });
        prompt.setFillColor(Color::White);
    }

    // Intro loop
    bool showIntro = true;
    Clock introClock;
    while (showIntro && window.isOpen())
    {
        while (true)
        {
            std::optional<Event> evOpt = window.pollEvent();
            if (!evOpt.has_value())
                break;

            const Event& ev = evOpt.value();
            if (ev.is<Event::Closed>())
            {
                window.close();
            }
            else if (ev.is<Event::KeyPressed>())
            {
                showIntro = false; // any key to proceed
            }
        }

        // Auto‐advance after 5 seconds
        if (introClock.getElapsedTime().asSeconds() > 5.f)
            break;

        window.clear({10, 10, 70});
        window.draw(title);
        window.draw(prompt);
        window.display();
    }

    // 4) For textures, use loadFromFile(...) in SFML 3.0.1
    Texture idleTexture, moveTexture1, moveTexture2;
    if (!idleTexture.loadFromFile("/Users/nicolasvargas/Desktop/Super-Mario-Bros-Main/Source/FotosMC/mathi.png") ||
        !moveTexture1.loadFromFile("/Users/nicolasvargas/Desktop/Super-Mario-Bros-Main/Source/FotosMC/mathi_movimiento.png") ||
        !moveTexture2.loadFromFile("/Users/nicolasvargas/Desktop/Super-Mario-Bros-Main/Source/FotosMC/mathi_movimiento_complemento.png"))
    {
        return -1;
    }

    // Player sprite (scaled down)
    Sprite player(idleTexture);
    {
        FloatRect b = player.getLocalBounds();
        // Use .size.x and .size.y for sf::FloatRect dimensions
        player.setOrigin({ b.size.x * 0.5f, b.size.y * 0.5f });
    }
    player.setScale({ 0.1f, 0.1f });
    player.setPosition({ 800.f, 450.f });

    // View (camera)
    View view = window.getDefaultView();
    view.setCenter(player.getPosition());

    // Build map: ground + floating platforms
    std::vector<RectangleShape> platforms;
    auto makePlat = [&](float x, float y, float w, float h)
    {
        RectangleShape r({ w, h });
        r.setPosition({ x, y });
        r.setFillColor(Color(100, 100, 200));
        platforms.push_back(r);
    };

    // Extend ground (100 segments × 400px = 40000px)
    for (int i = 0; i < 100; ++i)
        makePlat(i * 400.f, 880.f, 400.f, 20.f);

    // Floating platforms
    makePlat(600.f, 700.f, 300.f, 20.f);
    makePlat(1400.f, 600.f, 250.f, 20.f);
    makePlat(2000.f, 500.f, 200.f, 20.f);
    makePlat(2600.f, 650.f, 350.f, 20.f);

    // Physics constants
    const float GRAVITY  = 1200.f;
    const float MOVE_SPD =  300.f;
    const float JUMP_SPD =  600.f;

    Vector2f velocity{ 0.f, 0.f };
    bool onGround = false;

    // Animation toggle
    bool  toggleMoveFrame = false;
    float toggleInterval  = 0.15f;
    Clock toggleClock;

    // Delta-time clock
    Clock dtClock;

    // Main loop
    while (window.isOpen())
    {
        float dt = dtClock.restart().asSeconds();

        // Process events (SFML 3 style)
        while (true)
        {
            std::optional<Event> evOpt = window.pollEvent();
            if (!evOpt.has_value()) break;

            const Event& ev = evOpt.value();
            if (ev.is<Event::Closed>())
                window.close();
        }

        // INPUT
        bool isMoving = false;
        velocity.x = 0.f;

        if (Keyboard::isKeyPressed(Keyboard::Key::Left))
        {
            velocity.x = -MOVE_SPD;
            player.setScale({ 0.1f, 0.1f });
            isMoving = true;
        }
        else if (Keyboard::isKeyPressed(Keyboard::Key::Right))
        {
            velocity.x = MOVE_SPD;
            player.setScale({ -0.1f, 0.1f });
            isMoving = true;
        }
        if (Keyboard::isKeyPressed(Keyboard::Key::Up) && onGround)
        {
            velocity.y = -JUMP_SPD;
            onGround = false;
        }

        // ANIMATION
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

        // PHYSICS
        velocity.y += GRAVITY * dt;

        // Move X + collisions
        player.move({ velocity.x * dt, 0.f });
        {
            FloatRect pb = player.getGlobalBounds();
            // Use .position.x, .position.y, .size.x, .size.y for sf::FloatRect
            float pbLeft   = pb.position.x;
            float pbRight  = pb.position.x + pb.size.x;
            float pbTop    = pb.position.y;
            float pbBottom = pb.position.y + pb.size.y;


            for (auto &plat : platforms)
            {
                FloatRect pr = plat.getGlobalBounds();
                // Use .position.x, .position.y, .size.x, .size.y for sf::FloatRect
                float prLeft   = pr.position.x;
                float prRight  = pr.position.x + pr.size.x;
                float prTop    = pr.position.y;
                float prBottom = pr.position.y + pr.size.y;

                bool hitX =
                    (pbRight > prLeft) &&
                    (pbLeft < prRight) &&
                    (pbBottom > prTop) &&
                    (pbTop < prBottom);

                if (hitX)
                {
                    if (velocity.x > 0.f)
                        player.setPosition({ prLeft - pb.size.x * 0.5f, player.getPosition().y });
                    else if (velocity.x < 0.f)
                        player.setPosition({ prRight + pb.size.x * 0.5f, player.getPosition().y });

                    velocity.x = 0.f;
                    pb = player.getGlobalBounds(); // Re-fetch bounds after position change
                }
            }
        }

        // Move Y + collisions
        player.move({ 0.f, velocity.y * dt });
        onGround = false;
        {
            FloatRect pb = player.getGlobalBounds();
            // Use .position.x, .position.y, .size.x, .size.y for sf::FloatRect
            float pbLeft   = pb.position.x;
            float pbRight  = pb.position.x + pb.size.x;
            float pbTop    = pb.position.y;
            float pbBottom = pb.position.y + pb.size.y;

            for (auto &plat : platforms)
            {
                FloatRect pr = plat.getGlobalBounds();
                // Use .position.x, .position.y, .size.x, .size.y for sf::FloatRect
                float prLeft   = pr.position.x;
                float prRight  = pr.position.x + pr.size.x;
                float prTop    = pr.position.y;
                float prBottom = pr.position.y + pr.size.y;

                bool hitY =
                    (pbRight > prLeft) &&
                    (pbLeft < prRight) &&
                    (pbBottom > prTop) &&
                    (pbTop < prBottom);

                if (hitY)
                {
                    if (velocity.y > 0.f)
                    {
                        player.setPosition({ player.getPosition().x, prTop - pb.size.y * 0.5f });
                        onGround = true;
                    }
                    else if (velocity.y < 0.f)
                    {
                        player.setPosition({ player.getPosition().x, prBottom + pb.size.y * 0.5f });
                    }
                    velocity.y = 0.f;
                }
            }
        }

        // SCROLL VIEW
        view.setCenter(player.getPosition());
        window.setView(view);

        // RENDER
        window.clear({180, 220, 255});
        for (auto &plat : platforms)
            window.draw(plat);
        window.draw(player);
        window.display();
    }

    return 0;
}