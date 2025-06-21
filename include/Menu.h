#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <string>
#include "LevelManager.h"

class Menu {
public:
    Menu(sf::RenderWindow& window, const sf::Font& font, sf::VideoMode& mode, const sf::Texture& backgroundSprite, sf::SoundBuffer& clickBuffer);

    void handleEvent(LevelManager& levelManager);
    void update();
    void render();

    bool shouldStartGame() const;
    float getVolume() const;

    void updateBackgroundScale();

private:
    sf::RenderWindow& window;
    const sf::Font& font;

    sf::Sound clickSound;

    sf::Sprite backgroundSprite;
    sf::VideoMode& mode;
    sf::Vector2u textureSize;

    sf::Text titleText;
    sf::Text playButton;
    sf::Text volumeLabel;
    sf::Text resolutionLabel;

    std::size_t currentResolutionIndex;

    float volume;
    bool startGame;

    sf::RectangleShape selectorBox;

    void updateTexts();
    bool isMouseOverText(const sf::Text& text, const sf::Vector2f& mousePos);

    std::vector<sf::VideoMode> resolutions = {
        mode,
        sf::VideoMode::getDesktopMode()
    };
    bool fullscreen = false;

    std::vector<std::string> resolutionsOp;
};