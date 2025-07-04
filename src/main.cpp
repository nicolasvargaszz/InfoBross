#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <optional>
#include <vector>
#include <algorithm>
#include <string>
#include <iostream>
#include "../include/cafe.h"
#include "../include/TiledMap.h"
#include "../include/Player.h"
#include "../include/LevelManager.h"
#include "../include/FadeTransition.h"
#include "../include/Dialogue.h"
#include "../include/Menu.h" 
#include "../include/ExitButton.h" 
#include "../include/GameOver.h"

using namespace sf;

// Assets paths
std::vector<std::string> maps = {
    "../assets/maps/map1.json",
    "../assets/maps/map2.json",
    "../assets/maps/porfavor.json"
};
std::vector<std::string> tilesetTextures = {
    "../assets/tilesets/Terrain (32x32).png",
    "../assets/tilesets/Decorations (32x32).png",
    "../assets/tilesets/Idle.png"
};


enum class GameState {
    MENU,
    DIALOGUE,
    PLAYING,
    GAME_OVER, 
    WIN
};

int main()
{
    // --- Tiled Map ---
    LevelManager levelManager(maps, tilesetTextures);

    // --- Create Window ---
    sf::Vector2f mapSize = (*levelManager.getCurrentMap()).getPixelSize();
    VideoMode mode({
        static_cast<unsigned int>(std::min(mapSize.x, 1280.f)),
        static_cast<unsigned int>(std::min(mapSize.y, 720.f))
    });
    RenderWindow window(mode, "INFOBROS", sf::Style::None);
    window.setFramerateLimit(60);

    // --- Font ---
    Font font;
    if (!font.openFromFile("../assets/font/GAME_glm.ttf"))
        return -1;

    // --- Menu ---
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("../assets/tilesets/Menu_Background.png")) {
        std::cerr << "No se pudo cargar imagen de fondo\n";
        return -1;
    }

    // BACKGROUNDS

    // -- Start Background Image --
    sf::Texture dialogueBgTexture;
    if (!dialogueBgTexture.loadFromFile("../assets/tilesets/background_start.jpeg")) { // Use your actual path
        std::cerr << "No se pudo cargar el fondo de diálogo\n";
    }
    sf::Sprite dialogueBgSprite(dialogueBgTexture);
    dialogueBgSprite.setScale({
    float(window.getSize().x) / dialogueBgTexture.getSize().x,
    float(window.getSize().y) / dialogueBgTexture.getSize().y
    });
    dialogueBgSprite.setPosition({0.f, 0.f});

    sf::Texture winBgTexture;
    if (!winBgTexture.loadFromFile("../assets/tilesets/background_winning.jpeg")) { // Usa tu ruta real
        std::cerr << "No se pudo cargar el fondo de victoria\n";
    }
    sf::Sprite winBgSprite(winBgTexture);
    winBgSprite.setScale({
        float(window.getSize().x) / winBgTexture.getSize().x,
        float(window.getSize().y) / winBgTexture.getSize().y
    });
    winBgSprite.setPosition({0.f, 0.f});

    //SOUNDS EFECTS

    // --- Sounds Effects ---
    sf::SoundBuffer clickBuffer;
    if (!clickBuffer.loadFromFile("../assets/sounds/game_click.mp3")) {
    std::cerr << "Error cargando sonido de clic.\n";
    }
    // --- kill the enemy sound ---
    sf::SoundBuffer killEnemyBuffer;
    if (!killEnemyBuffer.loadFromFile("../assets/sounds/kill_enemy.mp3")) {
        std::cerr << "Error cargando sonido de matar enemigo.\n";
    }
    sf::Sound killEnemySound(killEnemyBuffer);
    levelManager.getCurrentMap()->setKillEnemySound(&killEnemySound);

    // --- GAME Over Sound ---
    sf::SoundBuffer deathBuffer;
    if (!deathBuffer.loadFromFile("../assets/sounds/DEATH.mp3")) {
        std::cerr << "No se pudo cargar el sonido de muerte\n";
    }
    sf::Sound deathSound(deathBuffer);
    levelManager.getCurrentMap()->setDeathSound(&deathSound);

    // --- CAFE Sound ---
    sf::SoundBuffer cafeBuffer;
    if (!cafeBuffer.loadFromFile("../assets/sounds/CAFE.mp3")) {
        std::cerr << "No se pudo cargar el sonido de café\n";
    }
    sf::Sound cafeSound(cafeBuffer);
    Cafe::setCollectSound(&cafeSound);
    Menu menu(window, font, mode, backgroundTexture, clickBuffer);


    // --- WINNING Sound ---
    sf::SoundBuffer winBuffer;
    if (!winBuffer.loadFromFile("../assets/sounds/WINNING.mp3")) {
        std::cerr << "No se pudo cargar el sonido de victoria\n";
    }
    sf::Sound winSound(winBuffer);
    
    // --- Botón de Salir ---
    ExitButton exitButton(window, font);


    // --- Music ---
    Music backgroundMusic;
    if (!backgroundMusic.openFromFile("../assets/sounds/musica_OOP.mp3"))
        return -1;
    backgroundMusic.setLooping(true);
    backgroundMusic.setVolume(20.f);
    backgroundMusic.play();

    // --- Fade Transition ---
    FadeTransition fade(sf::Vector2u(mapSize), sf::seconds(0.83f));

    // --- Dialogue Scene Assets ---
    Texture staticMathiTexture;
    if (!staticMathiTexture.loadFromFile("../assets/sprites/static_mathi.png")){
        std::cout << "Failed to load static_mathi.png";
        return -1;
    }
    std::vector<std::string> dialogueLines = {
        "Oh, I fell asleep!!!!! ",
        "I need to pass Data structures and OOP...",
        "I will study i must pass this course..."
    };
    std::vector<std::string> wakeUpSprites = {
    "../assets/sprites/Mathiwakeup/WAKEUP_0.png",
    "../assets/sprites/Mathiwakeup/WAKE_UP1.png",
    "../assets/sprites/Mathiwakeup/WAKE_UP2.png",
    "../assets/sprites/studying_mathi/study_0.png",
    "../assets/sprites/studying_mathi/study_1.png",
    "../assets/sprites/studying_mathi/study_2.png",
    "../assets/sprites/studying_mathi/study_3.png",
    "../assets/sprites/studying_mathi/study_4.png",
    "../assets/sprites/studying_mathi/study_5.png",
    };
    Dialogue dialogue(font, dialogueLines, 20U, wakeUpSprites);

    // --- Title Screen Text ---
    Text title_text(font, "INFOBROS", 128U);
    {
        FloatRect tb = title_text.getLocalBounds();
        title_text.setOrigin({tb.size.x * 0.5f, tb.size.y * 0.5f});
        title_text.setPosition({
            window.getSize().x * 0.5f,
            window.getSize().y * 0.5f
        });
        title_text.setFillColor(Color::Yellow);
    }
    Text prompt(font, "Press any key to start", 32U);
    {
        FloatRect pb_prompt_bounds = prompt.getLocalBounds();
        prompt.setOrigin({
            pb_prompt_bounds.size.x * 0.5f,
            pb_prompt_bounds.size.y * 0.5f
        });
        prompt.setPosition({
            window.getSize().x * 0.5f,
            window.getSize().y * 0.7f
        });
        prompt.setFillColor(Color::White);
    }


    // -- Message and window for winning the game --
    std::vector<std::string> winLines = {
        "Congratulations!",
        "You have completed the game.",
        "Thank you for playing!"
    };

    sf::Texture winTexture;
    if (!winTexture.loadFromFile("../assets/sprites/static_mathi.png")) {
        std::cerr << "No se pudo cargar imagen ganar\n";
        return -1;
    }

    std::vector<std::string> happySprites = {
    "../assets/sprites/happy_mathi/happy_0.png",
    "../assets/sprites/happy_mathi/happy_1.png"
    };
    Dialogue winDialogue(font, winLines, 20U, happySprites);

    // -- Message and window for losing the game --
    std::vector<std::string> gameOverLines = {
        "Game Over!",
        "You were defeated."
    };

    sf::Texture gameOverTexture;
    if (!gameOverTexture.loadFromFile("../assets/sprites/static_mathi.png")) {
        std::cerr << "No se pudo cargar imagen perder\n";
        return -1;
    }

    std::vector<std::string> crySprites = {
    "../assets/sprites/crying_mathi/cry_0.png",
    "../assets/sprites/crying_mathi/cry_1.png",
    "../assets/sprites/crying_mathi/cry_2.png",
    "../assets/sprites/crying_mathi/cry_3.png",
    "../assets/sprites/crying_mathi/cry_4.png",
    "../assets/sprites/crying_mathi/cry_5.png",
    "../assets/sprites/crying_mathi/cry_6.png",
    "../assets/sprites/crying_mathi/cry_7.png",
    };
    
    Dialogue gameOverDialogue(font, gameOverLines, 20U, crySprites);

    GameOverScreen gameOverScreen(font, gameOverLines, window.getSize());

    // --- Gameplay Assets ---
    Texture playerTexture;
    if (!playerTexture.loadFromFile("../assets/sprites/static_mathi.png")) {
        std::cerr << "Failed to load player texture" << std::endl;
        return -1;
    }

    // --- Player ---
    Player player(playerTexture);
    levelManager.getCurrentMap()->setPlayer(&player);
    if (!player.loadAnimation("idle", "../assets/sprites/static_mathi.png", 1, 0.5f) ||
        !player.loadAnimation("walk", "../assets/sprites/move_mathi.png", 4, 0.15f) ||
        !player.loadAnimation("jump", "../assets/sprites/jump_mathi.png", 1, 0.1f))
    {
        std::cerr << "Error loading player animations" << std::endl;
        return -1;
    }
    player.setPosition((*levelManager.getCurrentMap()).getEntradaPosition());

    // --- Cafes ---
    int cafeCounter = 0;
    bool gameFinished = false;
    sf::Text cafeScoreText(font, "CAFE: ", 30U);
    cafeScoreText.setFillColor(sf::Color::Blue);
    cafeScoreText.setPosition({30.f, 30.f});

    Clock dtClock;
    View view = window.getDefaultView();
    GameState state = GameState::MENU;

    while (window.isOpen())
    {
        float dt = dtClock.restart().asSeconds();

        std::optional<Event> evOpt = window.pollEvent();
        while (evOpt.has_value())
        {
            const Event& ev = evOpt.value();
            exitButton.handleEvent(ev);
            if (exitButton.isClicked())
                window.close();
            switch(state) // events switch
            {
                case GameState::MENU:
                    menu.handleEvent(levelManager); 
                    if (menu.shouldStartGame()) { 
                        state = GameState::DIALOGUE;
                        backgroundMusic.setVolume(menu.getVolume());
                    }
                    break;

                case GameState::DIALOGUE:
                    dialogue.handleEvent(ev);
                    break;

                case GameState::PLAYING:
                    break;
                
                case GameState::GAME_OVER:
                    gameOverScreen.handleEvent(ev);
                    break;
                
                case GameState::WIN:
                    winDialogue.handleEvent(ev);
                    break;
            }

            evOpt = window.pollEvent();
        }

        exitButton.update();

        switch(state) //main logic/update
        {
            case GameState::MENU:
            {
                menu.update();
                menu.render();
                break;
            }
            case GameState::DIALOGUE:
            {
                dialogue.update(dt);
                if (dialogue.isFinished())
                    state = GameState::PLAYING;
                break;
            }
            case GameState::PLAYING:
            {
                if (!gameFinished)
                {
                    player.handleInput();
                    player.applyPhysics(dt, *levelManager.getCurrentMap());
                    player.update(dt);
                    if (levelManager.getCurrentMap()->update(dt))
                    {
                        state = GameState::GAME_OVER;
                        gameOverScreen.reset();
                    };
                    fade.update();

                    levelManager.getCurrentMap()->getPuertaSalida().update(dt);

                    // Cámara
                    sf::Vector2f playerPos = player.getPosition();
                    sf::Vector2f viewSize = view.getSize();
                    sf::Vector2f mapSize = levelManager.getCurrentMap()->getPixelSize();
                    sf::Vector2f halfViewSize = viewSize / 2.f;
                    sf::Vector2f center = playerPos;

                    center.x = std::clamp(center.x, halfViewSize.x, mapSize.x - halfViewSize.x);
                    center.y = std::clamp(center.y, halfViewSize.y, mapSize.y - halfViewSize.y);
                    view.setCenter(center);
                    window.setView(view);

                    for (auto& c : levelManager.getCurrentMap()->getCafes())
                        c.update(player.getBounds(), cafeCounter, gameFinished);

                    // 2) Refresh the on-screen text only when it changes
                    cafeScoreText.setString("CAFE: " + std::to_string(cafeCounter));

                    // 3) If your Cafe::update sets gameFinished when you hit your target: game won
                    if (gameFinished)
                    {
                        state = GameState::WIN;
                        winDialogue.reset();
                        winSound.play();
                    }


                    // Puerta
                    static bool waitingForDoor = false;
                    static Clock doorTimer;
                    if (!waitingForDoor && levelManager.getCurrentMap()->isTouchingSalida(player.getBounds()))
                    {
                        levelManager.getCurrentMap()->getPuertaSalida().play();
                        doorTimer.restart();
                        fade.start();
                        waitingForDoor = true;
                    }
                    else if (waitingForDoor && doorTimer.getElapsedTime().asSeconds() > 0.5f)
                    {
                        levelManager.loadNextLevel();
                        levelManager.getCurrentMap()->setPlayer(&player);
                        player.setPosition(levelManager.getCurrentMap()->getEntradaPosition());
                        waitingForDoor = false;
                    }
                }
                break;
            }
            case GameState::GAME_OVER:
            {
                gameOverScreen.update(dt);
                if (gameOverScreen.isFinished()) 
                {
                    state = GameState::MENU; 
                }
                break;
            }
            case GameState::WIN:
            {
                winDialogue.update(dt);
                if (winDialogue.isFinished()) {
                    state = GameState::MENU;
                    winDialogue.reset();
                }
                break;
            }
        }

        window.clear();

        switch(state)
        {
            case GameState::MENU:
                menu.render(); 
                break;

            case GameState::DIALOGUE:
                window.setView(window.getDefaultView());
                dialogueBgSprite.setScale({
                float(window.getSize().x) / dialogueBgTexture.getSize().x,
                float(window.getSize().y) / dialogueBgTexture.getSize().y
                });
                dialogueBgSprite.setPosition({0.f, 0.f});
                window.draw(dialogueBgSprite);
                dialogue.draw(window);
                break;

            case GameState::PLAYING:
                levelManager.draw(window);
                window.draw(*levelManager.getCurrentMap());
                window.draw(levelManager.getCurrentMap()->getPuertaSalida());

                for (auto& c : levelManager.getCurrentMap()->getCafes())
                    c.render(window);

                // draw your counter
                window.draw(cafeScoreText);
            

                player.render(window);
                fade.draw(window);
                break;

            case GameState::GAME_OVER:
                window.setView(window.getDefaultView());
                winBgSprite.setScale({
                    float(window.getSize().x) / winBgTexture.getSize().x,
                    float(window.getSize().y) / winBgTexture.getSize().y
                });
                winBgSprite.setPosition({0.f, 0.f});
                window.draw(winBgSprite);
                gameOverScreen.draw(window);
                break;
            case GameState::WIN:
                window.setView(window.getDefaultView());
                winBgSprite.setScale({
                    float(window.getSize().x) / winBgTexture.getSize().x,
                    float(window.getSize().y) / winBgTexture.getSize().y
                });
                winBgSprite.setPosition({0.f, 0.f});
                window.draw(winBgSprite);
                winDialogue.draw(window);
                break;
        }

        if (state != GameState::MENU)
        {
            window.setView(window.getDefaultView());
            exitButton.render();
        }
        if (state == GameState::PLAYING)
        {
            window.draw(cafeScoreText);
        }
        window.display();
    }

    return 0;
}
