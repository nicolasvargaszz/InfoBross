#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <unordered_set>
#include "AnimatedDoor.h"
#include "Enemy.h"
#include "Cafe.h"

class Player; // Forward declaration para evitar ciclo de includes

class TiledMap : public sf::Drawable
{
public:
    // Constructor recibe ruta del mapa JSON y vector con rutas de texturas (tilesets)
    TiledMap(const std::string& jsonMapPath, const std::vector<std::string>& texturePaths);

    // Access spawned cafés
    std::vector<Cafe>& getCafes() { return cafes; }
    // Para detectar si el jugador está tocando la puerta de salida
    bool isTouchingSalida(const sf::FloatRect& bounds) const;

    // Para obtener la posición de la puerta de entrada donde aparece el jugador
    sf::Vector2f getEntradaPosition() const;

    // Colisión con tiles sólidos
    bool isColliding(const sf::FloatRect& bounds) const;

    sf::Vector2f getPixelSize() const;

    void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override;

    // Door
    void update(float dt);
    AnimatedDoor& getPuertaEntrada();
    AnimatedDoor& getPuertaSalida();

    // player
    void setPlayer(Player* p) { playerPtr = p; }
    

private:
    int mapWidth;
    int mapHeight;
    int tileWidth;
    int tileHeight;

    //cafes
    sf::Texture cafeTexture;
    std::vector<Cafe> cafes;
    
    // Door
    AnimatedDoor puertaEntrada;
    AnimatedDoor puertaSalida;
    Animation animEntrada;
    Animation animSalida;
    sf::Texture texEntrada;
    sf::Texture texSalida;

    sf::Texture puertaTexture;
    std::vector<sf::Texture> tilesetTextures;

    std::vector<sf::Sprite> tiles;           // Todos los tiles para dibujar
    std::vector<sf::Sprite> solidTiles;      // Tiles sólidos para colisión
    std::vector<sf::FloatRect> solidRects;  // Rectángulos sólidos

    //Enemies

    std::vector<Enemy> enemies;  // <-- Store Enemy objects here
    
    sf::Texture enemyTexture;

    sf::Vector2f entradaPosition;            // Posición de la puerta de entrada
    std::vector<sf::FloatRect> salidaRects;  // Rectángulos de puertas de salida

    std::unordered_set<int> solidGIDs = {
        20,21,22,24,26,27,29,30,32,33,35,
        39,41,43,45,46,48,49,51,52,54,55,
        58,59,60,62,83,84,86,87,89,90,92,
        93,96,97,98,100,102,103,105,106,
        108,109,111,112
    };

    Player* playerPtr = nullptr; // Puntero al jugador para colisiones con enemigos


};
