#include "../include/FadeTransition.h"
#include <cstdint>

FadeTransition::FadeTransition(sf::Vector2u windowSize, sf::Time duration)
    : totalDuration(duration),
      halfDuration(duration / 2.f),
      currentState(State::None)
{
    fadeRect.setSize(sf::Vector2f(windowSize));
    fadeRect.setFillColor(sf::Color(0, 0, 0, 0));
}

void FadeTransition::start() {
    clock.restart();
    currentState = State::FadingOut;
}

void FadeTransition::update() {
    if (currentState == State::None || currentState == State::Done) return;

    sf::Time elapsed = clock.getElapsedTime();
    float progress = 0.f;

    if (elapsed < halfDuration) {
        progress = elapsed.asSeconds() / halfDuration.asSeconds();
        fadeRect.setFillColor(sf::Color(0, 0, 0, static_cast<std::uint8_t>(progress * 255)));
        currentState = State::FadingOut;
    } else if (elapsed < totalDuration) {
        progress = (elapsed - halfDuration).asSeconds() / halfDuration.asSeconds();
        fadeRect.setFillColor(sf::Color(0, 0, 0, static_cast<std::uint8_t>((1 - progress) * 255)));
        currentState = State::FadingIn;
    } else {
        fadeRect.setFillColor(sf::Color(0, 0, 0, 0));
        currentState = State::Done;
    }
}

void FadeTransition::draw(sf::RenderWindow& window) {
    if (currentState != State::None && currentState != State::Done)
        window.draw(fadeRect);
}

bool FadeTransition::isRunning() const {
    return currentState != State::None && currentState != State::Done;
}

FadeTransition::State FadeTransition::getState() const {
    return currentState;
}