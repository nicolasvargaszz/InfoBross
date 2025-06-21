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

using namespace sf;

// Assets paths
std::vector<std::string> maps = {
    "../assets/maps/Mapa2.json",
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
    GAME_OVER
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

    sf::SoundBuffer clickBuffer;
    if (!clickBuffer.loadFromFile("../assets/sounds/game_click.mp3")) {
    std::cerr << "Error cargando sonido de clic.\n";
    }

    Menu menu(window, font, mode, backgroundTexture, clickBuffer);

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
    Dialogue dialogue(font, dialogueLines, 20U, staticMathiTexture);

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

    Texture cafeTexture;
    if (!cafeTexture.loadFromFile("../assets/sprites/CAFE.png"))
        return -1;

    std::vector<Sprite> cafeItems;
    int cafeScore = 0;
    const int CAFE_TO_WIN = 10;
    bool gameWon = false;
    int cafeCounter = 0;
    bool gameFinished = false;

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

            switch(state)
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
                    if (ev.is<Event::KeyPressed>())
                        window.close();
                    break;
            }

            evOpt = window.pollEvent();
        }

        exitButton.update();

        switch(state)
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
                if (!gameWon)
                {
                    player.handleInput();
                    player.applyPhysics(dt, *levelManager.getCurrentMap());
                    player.update(dt);
                    levelManager.getCurrentMap()->update(dt);
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
                break;
        }

        window.clear();

        switch(state)
        {
            case GameState::MENU:
                menu.render(); 
                break;

            case GameState::DIALOGUE:
                dialogue.draw(window);
                break;

            case GameState::PLAYING:
                levelManager.draw(window);
                window.draw(*levelManager.getCurrentMap());
                window.draw(levelManager.getCurrentMap()->getPuertaSalida());

                for (auto& cafe : cafeItems)
                    window.draw(cafe);

                player.render(window);
                fade.draw(window);
                break;

            case GameState::GAME_OVER:
                break;
        }

        exitButton.render();
        window.display();
    }

    return 0;
}
