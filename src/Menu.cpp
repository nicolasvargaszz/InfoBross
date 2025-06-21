#include "../include/Menu.h"
#include <cmath>
#include <iostream>

Menu::Menu(sf::RenderWindow& window, const sf::Font& font, sf::VideoMode& m, const sf::Texture& bSprite, sf::SoundBuffer& clickBuffer)
    : window(window), font(font), currentResolutionIndex(0), volume(50.f), startGame(false), mode(m), backgroundSprite(bSprite)
    , titleText(font, "Main Menu" , 36U)
    , playButton(font, "Play" , 28U)
    , volumeLabel(font, "Volume" , 24U)
    , resolutionLabel(font, "Resolution" , 24U)
    , exitLabel(font, "Exit" , 24U)
    , clickSound(clickBuffer)
{
    textureSize = bSprite.getSize();
    
    updateBackgroundScale();

    resolutionsOp  = {
        "Windowed",
        "FullScrean"
    };

    updateTexts();
}

void Menu::updateBackgroundScale()
{
    sf::Vector2u windowSize = window.getSize();
    if (textureSize.x == 0 || textureSize.y == 0) return; 

    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
    backgroundSprite.setScale({scaleX, scaleY});
}

void Menu::handleEvent(LevelManager& levelManager)
{
    while (auto evOpt = window.pollEvent())
    {
        const sf::Event& ev = *evOpt;
        if (auto* mouse = ev.getIf<sf::Event::MouseButtonPressed>())
        {
            if (mouse->button == sf::Mouse::Button::Left)
            {
                sf::Vector2f mousePos = window.mapPixelToCoords(mouse->position);

                if (isMouseOverText(playButton, mousePos)) {
                    clickSound.play(); 
                    startGame = true;
                } 
                else if (isMouseOverText(volumeLabel, mousePos)) {
                    clickSound.play(); 
                    volume = std::fmod(volume + 10.f, 110.f);
                    updateTexts();
                } 
                else if (isMouseOverText(resolutionLabel, mousePos)) {
                    clickSound.play(); 
                    currentResolutionIndex = (currentResolutionIndex + 1) % resolutions.size();

                    window.create(resolutions[currentResolutionIndex], "INFOBROS", sf::Style::None);
                    window.setFramerateLimit(60);

                    updateBackgroundScale();
                    updateTexts();
                }
                else if (isMouseOverText(exitLabel, mousePos)) {
                    clickSound.play(); 
                    window.close();
                }
            }
        }
    }
}

void Menu::update() {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    
    std::vector<sf::Text*> options = {&playButton, &volumeLabel, &resolutionLabel, &exitLabel};
    
    for (auto* textPtr : options)
    {
        if (isMouseOverText(*textPtr, mousePos)) {
            if (textPtr == &exitLabel){
                textPtr->setFillColor(sf::Color::Red); 
            } else {
                textPtr->setFillColor(sf::Color::Yellow);
            }
            textPtr->setScale({1.1f, 1.1f}); 
        } else {
            textPtr->setFillColor(sf::Color::White);
            textPtr->setScale({1.f, 1.f});  
        }
    }
}

void Menu::render()
{
    window.draw(backgroundSprite);
    window.draw(titleText);
    window.draw(playButton);
    window.draw(volumeLabel);
    window.draw(resolutionLabel);
    window.draw(exitLabel);
}

bool Menu::shouldStartGame() const {
    return startGame;
}

float Menu::getVolume() const {
    return volume;
}

void Menu::updateTexts()
{
    // Actualizamos textos con los valores
    volumeLabel.setString("Volume: " + std::to_string(static_cast<int>(volume)) + "%");
    resolutionLabel.setString("Resolution: " + resolutionsOp[currentResolutionIndex]);

    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setOrigin({titleBounds.position.x + titleBounds.size.x / 2.f, titleBounds.position.y + titleBounds.size.y / 2.f});
    titleText.setPosition({window.getSize().x / 2.f, 75.f});

    std::vector<sf::Text*> options = {&playButton, &volumeLabel, &resolutionLabel, &exitLabel};

    float spacingY = 60.f; // espacio vertical entre opciones
    float startY = 220.f;  // posición Y inicial para el primer botón

    for (auto* textPtr : options)
    {
        sf::Text& text = *textPtr;

        sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin({bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f});

        text.setPosition({window.getSize().x / 2.f, startY});

        startY += spacingY;
    }

    
    sf::Vector2u windowSize = window.getSize();

    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
    backgroundSprite.setScale({scaleX, scaleY});
}

bool Menu::isMouseOverText(const sf::Text& text, const sf::Vector2f& mousePos)
{
    return text.getGlobalBounds().contains(mousePos);
}
