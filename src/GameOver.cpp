#include "../include/GameOver.h"

GameOverScreen::GameOverScreen(const sf::Font& font, const std::vector<std::string>& lines, sf::Vector2u windowSize)
    : dialogue(const_cast<sf::Font&>(font), lines, 32, std::vector<std::string>{
        "../assets/sprites/crying_mathi/cry_0.png",
        "../assets/sprites/crying_mathi/cry_1.png",
        "../assets/sprites/crying_mathi/cry_2.png",
        "../assets/sprites/crying_mathi/cry_3.png",
        "../assets/sprites/crying_mathi/cry_4.png",
        "../assets/sprites/crying_mathi/cry_5.png",
        "../assets/sprites/crying_mathi/cry_6.png",
        "../assets/sprites/crying_mathi/cry_7.png"
    }),
      retryButton(font),
      finished(false)
{
    retryButton.setFont(font);
    retryButton.setString("Play Again");
    retryButton.setCharacterSize(36);
    retryButton.setFillColor(sf::Color::White);
    sf::FloatRect rect = retryButton.getLocalBounds();
    retryButton.setOrigin({rect.size.x / 2.f, rect.size.y / 2.f});
    retryButton.setPosition({windowSize.x / 2.f, windowSize.y / 2.f + 120.f});
}

void GameOverScreen::update(float dt) {
    if (!dialogue.isFinished())
        dialogue.update(dt);
    else
        finished = true;
}

void GameOverScreen::handleEvent(const sf::Event& ev) {
    if (!dialogue.isFinished()) {
        dialogue.handleEvent(ev);
    }
}

void GameOverScreen::draw(sf::RenderWindow& window) {
    dialogue.draw(window);
    if (dialogue.isFinished()) {
        sf::Vector2i mouse = sf::Mouse::getPosition(window);
        if (retryButton.getGlobalBounds().contains(window.mapPixelToCoords(mouse)))
            retryButton.setFillColor(sf::Color::Yellow);
        else
            retryButton.setFillColor(sf::Color::White);
        window.draw(retryButton);
    }
}

bool GameOverScreen::isFinished() const {
    return finished;
}

void GameOverScreen::reset() {
    dialogue.reset();
    finished = false;
}