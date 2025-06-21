#include "../include/ExitButton.h"
#include <iostream>

ExitButton::ExitButton(sf::RenderWindow& win, const sf::Font& font)
    : window(win), clicked(false), buttonText(font, "X", 24U), font(font)
{}

void ExitButton::handleEvent(const sf::Event& event)
{
    if (auto* mouse = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (mouse->button == sf::Mouse::Button::Left)
        {
            // Obtener posición del mouse en vista por defecto
            sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);
            sf::Vector2f mousePos = window.mapPixelToCoords(mousePixelPos, window.getDefaultView());

            //std::cout << "Click en posición ventana: " << mousePixelPos.x << "," << mousePixelPos.y << " - Mapeado a coords: " << mousePos.x << "," << mousePos.y << std::endl;

            if (buttonText.getGlobalBounds().contains(mousePos))
            {
                //std::cout << "Botón clickeado!" << std::endl;
                clicked = true;
            }
        }
    }
}

void ExitButton::update()
{
    // Cambiar color al pasar el mouse (hover)
    sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f mousePos = window.mapPixelToCoords(mousePixelPos, window.getDefaultView());

    if (buttonText.getGlobalBounds().contains(mousePos))
        buttonText.setFillColor(sf::Color::Red);
    else
        buttonText.setFillColor(sf::Color::Black);
}

void ExitButton::render()
{
    // Fondo blanco para el exit button
    sf::RectangleShape header(sf::Vector2f(window.getSize().x, 30));
    header.setFillColor(sf::Color::White); // Gris oscuro
    header.setPosition({0, 0});
    window.draw(header);

    sf::Text title(font, "INFOBROS", 20);
    title.setFillColor(sf::Color::White);
    float scaleX = static_cast<float>(window.getSize().x) - 150;
    title.setPosition({scaleX, 10}); // Alinear a la derecha
    window.draw(title);

    // Dibujar el botón de salida
    sf::View originalView = window.getView();

    window.setView(window.getDefaultView());
    window.draw(buttonText);
    window.setView(originalView);
}

bool ExitButton::isClicked() const
{
    return clicked;
}


