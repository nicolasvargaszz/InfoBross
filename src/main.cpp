#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <optional>
#include <vector>
#include <algorithm>
#include <string>
#include <iostream>
#include "../include/TiledMap.h"

using namespace sf;

int main()
{
    // --- Tiled Map ---
    TiledMap tiledMap("../assets/maps/map1.json", "../Kings and Pigs/Sprites/Terrain (32x32).png");

    sf::Vector2f mapSize = tiledMap.getPixelSize();
    VideoMode mode({static_cast<unsigned int>(std::min(mapSize.x, 1280.f)), static_cast<unsigned int>(std::min(mapSize.y, 720.f))});
    RenderWindow window(mode, "INFOBROS");
    window.setFramerateLimit(60);

    // --- Font ---
    Font font;
    if (!font.openFromFile("../FotosMC/GAME_glm.ttf"))
        return -1;

    // --- Music ---
    /*Music backgroundMusic;
    if (!backgroundMusic.openFromFile("../FotosMC/musica_OOP.mp3"))
        return -1;
    backgroundMusic.setLooping(true);
    backgroundMusic.setVolume(1.f);
    backgroundMusic.play();*/

    // --- Dialogue Scene Assets ---
    Texture staticMathiTexture;
    if (!staticMathiTexture.loadFromFile("../FotosMC/static_mathi.png"))
        return -1;
    Sprite staticMathiSprite(staticMathiTexture);
    staticMathiSprite.setScale({-0.5f, 0.5f});
    FloatRect bounds = staticMathiSprite.getGlobalBounds();
    staticMathiSprite.setPosition({10.f + bounds.size.x, window.getSize().y - bounds.size.y - 10.f});

    std::vector<std::string> dialogueLines = {
        "Oh, I fell asleep!!!!! ",
        "I need to pass Data structures and OOP...",
        "I will study i must pass this course..."
    };
    int currentLineIndex = 0;
    std::string stringToDisplayForTypewriter = "";
    unsigned int currentCharTypewriterIndex = 0;
    Clock typewriterClock;
    const float TYPEWRITER_CHAR_INTERVAL = 0.05f;

    enum class DialogueState { TYPING, WAITING_FOR_NEXT, FINISHED };
    DialogueState currentDialogueState = DialogueState::TYPING;

    Text dialogueText(font, "", 35U);
    dialogueText.setFillColor(Color::White);
    FloatRect dialogueTextBounds = dialogueText.getLocalBounds();
    dialogueText.setOrigin({dialogueTextBounds.size.x / 2.f, dialogueTextBounds.size.y / 2.f});
    dialogueText.setPosition({window.getSize().x / 2.f, window.getSize().y * 0.85f});

    RectangleShape dialogueBox;
    dialogueBox.setSize({window.getSize().x * 0.8f, 150.f});
    dialogueBox.setFillColor({0, 0, 0, 150});
    dialogueBox.setOrigin({dialogueBox.getSize().x / 2.f, dialogueBox.getSize().y / 2.f});
    dialogueBox.setPosition({window.getSize().x / 2.f, window.getSize().y * 0.85f});

    // --- Dialogue Scene Loop ---
    bool showDialogueScene = !dialogueLines.empty();
    if (showDialogueScene)
        typewriterClock.restart();

    while (showDialogueScene && window.isOpen())
    {
        std::optional<Event> evOpt = window.pollEvent();
        while (evOpt.has_value())
        {
            const Event &ev = evOpt.value();
            if (ev.is<Event::Closed>())
            {
                window.close();
                showDialogueScene = false;
            }
            else if (ev.is<Event::KeyPressed>())
            {
                if (currentDialogueState == DialogueState::TYPING)
                {
                    stringToDisplayForTypewriter = dialogueLines[currentLineIndex];
                    currentCharTypewriterIndex = stringToDisplayForTypewriter.length();
                    dialogueText.setString(stringToDisplayForTypewriter + "\n(Press any key to continue)");
                    currentDialogueState = DialogueState::WAITING_FOR_NEXT;
                }
                else if (currentDialogueState == DialogueState::WAITING_FOR_NEXT)
                {
                    currentLineIndex++;
                    if (currentLineIndex < dialogueLines.size())
                    {
                        stringToDisplayForTypewriter = "";
                        currentCharTypewriterIndex = 0;
                        dialogueText.setString("");
                        currentDialogueState = DialogueState::TYPING;
                        typewriterClock.restart();
                    }
                    else
                    {
                        showDialogueScene = false;
                    }
                }
            }
            if (showDialogueScene)
                evOpt = window.pollEvent();
            else
                break;
        }

        if (currentDialogueState == DialogueState::TYPING && currentLineIndex < dialogueLines.size())
        {
            if (typewriterClock.getElapsedTime().asSeconds() > TYPEWRITER_CHAR_INTERVAL)
            {
                if (currentCharTypewriterIndex < dialogueLines[currentLineIndex].length())
                {
                    stringToDisplayForTypewriter += dialogueLines[currentLineIndex][currentCharTypewriterIndex];
                    dialogueText.setString(stringToDisplayForTypewriter);
                    currentCharTypewriterIndex++;
                    typewriterClock.restart();
                }
                else
                {
                    dialogueText.setString(stringToDisplayForTypewriter + "\n(Press any key to continue)");
                    currentDialogueState = DialogueState::WAITING_FOR_NEXT;
                }
            }
        }

        dialogueTextBounds = dialogueText.getLocalBounds();
        dialogueText.setOrigin({dialogueTextBounds.size.x / 2.f, dialogueTextBounds.size.y / 2.f});
        dialogueText.setPosition({window.getSize().x / 2.f, window.getSize().y * 0.85f});

        window.clear({30, 30, 80});
        window.draw(staticMathiSprite);
        window.draw(dialogueBox);
        window.draw(dialogueText);
        window.display();
    }

    // --- Title Screen ---
    Text title_text(font, "INFOBROS", 128U);
    {
        FloatRect tb = title_text.getLocalBounds();
        title_text.setOrigin({tb.size.x * 0.5f, tb.size.y * 0.5f});
        title_text.setPosition({window.getSize().x * 0.5f, window.getSize().y * 0.5f});
        title_text.setFillColor(Color::Yellow);
    }
    Text prompt(font, "Press any key to start", 32U);
    {
        FloatRect pb_prompt_bounds = prompt.getLocalBounds();
        prompt.setOrigin({pb_prompt_bounds.size.x * 0.5f, pb_prompt_bounds.size.y * 0.5f});
        prompt.setPosition({window.getSize().x * 0.5f, window.getSize().y * 0.7f});
        prompt.setFillColor(Color::White);
    }
    bool showIntro = window.isOpen();
    Clock introClock;
    while (showIntro && window.isOpen())
    {
        std::optional<Event> evOpt = window.pollEvent();
        while (evOpt.has_value())
        {
            const Event &ev = evOpt.value();
            if (ev.is<Event::Closed>())
            {
                window.close();
                showIntro = false;
            }
            else if (ev.is<Event::KeyPressed>())
            {
                showIntro = false;
            }
            if (showIntro)
                evOpt = window.pollEvent();
            else
                break;
        }
        if (!showIntro)
            break;
        window.clear({10, 10, 70});
        window.draw(title_text);
        window.draw(prompt);
        window.display();
    }

    // --- Gameplay Assets ---
    Texture idleTexture, moveTexture1, moveTexture2;
    if (!idleTexture.loadFromFile("../FotosMC/mathi.png") ||
        !moveTexture1.loadFromFile("../FotosMC/mathi_movimiento.png") ||
        !moveTexture2.loadFromFile("../FotosMC/mathi_movimiento_complemento.png"))
        return -1;

    Sprite player(idleTexture);
    {
        FloatRect b_player = player.getLocalBounds();
        player.setOrigin({b_player.size.x * 0.5f, b_player.size.y * 0.5f});
    }
    player.setScale({0.1f, 0.1f});
    player.setPosition({50.f, 500.f});

    // --- CAFE Assets ---
    Texture cafeTexture;
    if (!cafeTexture.loadFromFile("../FotosMC/CAFE.jpeg"))
        return -1;
    std::vector<Sprite> cafeItems;
    int cafeScore = 0;
    const int CAFE_TO_WIN = 10;
    bool gameWon = false;

    Text cafeScoreText(font, "CAFE: 0", 30U);
    cafeScoreText.setFillColor(Color::Blue);

    Text winMessageText(font, "Congratulations! You passed the course!", 35U);
    winMessageText.setFillColor(Color::Blue);
    FloatRect winTextBounds = winMessageText.getLocalBounds();
    winMessageText.setOrigin({winTextBounds.size.x / 2.f, winTextBounds.size.y / 2.f});

    View view = window.getDefaultView();

    // --- Físicas ---
    const float GRAVITY = 1100.f;
    const float MOVE_SPD = 200.f;
    const float JUMP_SPD = 650.f;
    Vector2f velocity{0.f, 0.f};
    bool onGround = false;
    bool toggleMoveFrame = false;
    float toggleInterval = 0.15f;
    Clock toggleClock;
    Clock dtClock;

    if (!window.isOpen())
        return 0;

    // --- Main Game Loop ---
    while (window.isOpen())
    {
        float dt = dtClock.restart().asSeconds();
        if (gameWon)
            dt = 0;

        std::optional<Event> evOpt_main = window.pollEvent();
        while (evOpt_main.has_value())
        {
            const Event &ev = evOpt_main.value();
            if (ev.is<Event::Closed>())
                window.close();

            if (gameWon && ev.is<Event::KeyPressed>())
            {
                if (ev.getIf<Event::KeyPressed>()->code == Keyboard::Key::Escape)
                    window.close();
            }
            evOpt_main = window.pollEvent();
        }

        if (!gameWon)
        {
            bool isMoving = false;
            velocity.x = 0.f;

            if (Keyboard::isKeyPressed(Keyboard::Key::Left))
            {
                velocity.x = -MOVE_SPD;
                player.setScale({0.1f, 0.1f});
                isMoving = true;
            }
            else if (Keyboard::isKeyPressed(Keyboard::Key::Right))
            {
                velocity.x = MOVE_SPD;
                player.setScale({-0.1f, 0.1f});
                isMoving = true;
            }
            if (Keyboard::isKeyPressed(Keyboard::Key::Up) && onGround)
            {
                velocity.y = -JUMP_SPD;
                onGround = false;
            }

            if (!isMoving)
            {
                toggleClock.restart();
                player.setTexture(idleTexture, true);
                FloatRect b_player = player.getLocalBounds();
                player.setOrigin({b_player.size.x * 0.5f, b_player.size.y * 0.5f});
            }
            else
            {
                if (toggleClock.getElapsedTime().asSeconds() >= toggleInterval)
                {
                    toggleMoveFrame = !toggleMoveFrame;
                    toggleClock.restart();
                }
                player.setTexture(toggleMoveFrame ? moveTexture1 : moveTexture2, true);
                FloatRect b_player = player.getLocalBounds();
                player.setOrigin({b_player.size.x * 0.5f, b_player.size.y * 0.5f});
            }

            // --- Físicas y colisiones ---
            velocity.y += GRAVITY * dt;

            // Movimiento horizontal
            player.move({velocity.x * dt, 0.f});
            if (tiledMap.isColliding(player.getGlobalBounds()))
            {
                player.move({-velocity.x * dt, 0.f});  // Retrocede si colisiona
            }

            // Movimiento vertical
            player.move({0.f, velocity.y * dt});
            if (tiledMap.isColliding(player.getGlobalBounds()))
            {
                player.move({0.f, -velocity.y * dt});  // Retrocede si colisiona
                onGround = true;  // Si colisiona hacia abajo, el personaje está en el suelo
                velocity.y = 0.f;
            }
            else
            {
                onGround = false;
            }

            // --- Colisión con el mapa Tiled ---
            // Descomenta si quieres colisión con el mapa Tiled
            // if (tiledMap.isColliding(player.getGlobalBounds())) {
            //     // Corrige la posición o impide el movimiento según tu lógica
            // }

            // --- CAFE Collection ---
            for (size_t i = 0; i < cafeItems.size(); ++i)
            {
                FloatRect playerBounds = player.getGlobalBounds();
                FloatRect cafeBounds = cafeItems[i].getGlobalBounds();
                if (playerBounds.findIntersection(cafeBounds).has_value())
                {
                    cafeItems.erase(cafeItems.begin() + i);
                    cafeScore++;
                    cafeScoreText.setString("CAFE: " + std::to_string(cafeScore));
                    --i;
                }
            }

            if (cafeScore >= CAFE_TO_WIN)
                gameWon = true;
        }

        // --- Cámara ---
        view.setCenter(player.getPosition());
        //window.setView(view);
        sf::Vector2f halfViewSize = view.getSize() / 2.f;
        sf::Vector2f center = player.getPosition();

        center.x = std::clamp(center.x, halfViewSize.x, mapSize.x - halfViewSize.x);
        center.y = std::clamp(center.y, halfViewSize.y, mapSize.y - halfViewSize.y);

        view.setCenter(center);

        // --- UI ---
        Vector2f viewTopLeft = view.getCenter() - view.getSize() / 2.f;
        cafeScoreText.setPosition(viewTopLeft + Vector2f({10.f, 10.f}));
        winMessageText.setPosition(view.getCenter());

        // --- Dibujado ---
        window.clear({180, 220, 255});
        window.draw(tiledMap); // Descomenta si quieres dibujar el mapa Tiled
        if (!gameWon)
        {
            for (auto &cafe : cafeItems)
                window.draw(cafe);
            window.draw(player);
        }
        window.draw(cafeScoreText);
        if (gameWon)
            window.draw(winMessageText);
        window.display();
    }
    return 0;
}