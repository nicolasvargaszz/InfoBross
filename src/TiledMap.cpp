#include "../include/TiledMap.h"
#include<SFML/Audio.hpp>
#include <nlohmann/json.hpp>
#include "../include/Player.h"
#include <fstream>
#include <iostream>

bool intersects(const sf::FloatRect& a, const sf::FloatRect& b)
{
    return a.position.x < b.position.x + b.size.x &&
           a.position.x + a.size.x > b.position.x &&
           a.position.y < b.position.y + b.size.y &&
           a.position.y + a.size.y > b.position.y;
}

TiledMap::TiledMap(const std::string& jsonMapPath, const std::vector<std::string>& texturePaths) : playerPtr(nullptr)
{
    // Cargar todas las texturas tileset
    for (const auto& path : texturePaths)
    {
        sf::Texture texture;
        if (!texture.loadFromFile(path))
        {
            std::cerr << "Error loading texture: " << path << std::endl;
            continue;
        }
        tilesetTextures.push_back(texture);
        std::cout << "Cargado tileset: " << path << std::endl;    
    }
    // texture of the enemy
    if (!enemyTexture.loadFromFile("../assets/sprites/pixil-frame-0.png"))
    {
        std::cerr << "Error loading Enemy.png for enemies\n";
    }
    // Cargar textura especial de puerta (Idle.png)
    if (!puertaTexture.loadFromFile("../assets/tilesets/Idle.png"))
    {
        std::cerr << "No se pudo cargar Idle.png (puerta)" << std::endl;
    }
    //Cargar textura de los cafes
    if (!cafeTexture.loadFromFile("../assets/sprites/CAFE.png")) {
        std::cerr << "Error loading CAFE.png for cafes\n";
    }
    // Abrir y parsear JSON
    std::ifstream file(jsonMapPath);
    if (!file.is_open())
    {
        std::cerr << "Could not open " << jsonMapPath << std::endl;
        return;
    }

    nlohmann::json mapData;  // this is the library for reading the json file.
    file >> mapData;

    tileWidth  = mapData["tilewidth"];
    tileHeight = mapData["tileheight"];
    mapWidth   = mapData["width"];
    mapHeight  = mapData["height"];

    // Obtener firstgid de cada tileset para calcular índices locales
    std::vector<int> firstGIDs;
    for (const auto& tileset : mapData["tilesets"])
    {
        firstGIDs.push_back(tileset["firstgid"]);
    }

    auto getTilesetIndex = [&](int gid) {
        for (int i = int(firstGIDs.size()) - 1; i >= 0; --i)
        {
            if (gid >= firstGIDs[i]) return i;
        }
        return 0;
    };

    // Procesar capas y tiles
    for (const auto& layer : mapData["layers"])
    {
        if (!layer.contains("data") || !layer["data"].is_array()) continue;

        auto dataArray = layer["data"];
        for (int i = 0; i < static_cast<int>(dataArray.size()); ++i)
        {
            int rawID = dataArray[i];
            if (rawID <= 0) continue;

            int tilesetIndex = getTilesetIndex(rawID);
            int firstGID = firstGIDs[tilesetIndex];
            int localID = rawID - firstGID;

            // Por defecto usar tileset
            sf::Sprite tile(puertaTexture);

            // Cafe
            if (rawID == 293)
            {
                    int x = i % mapWidth;
                    int y = i / mapWidth;
                    float posX = float(x * tileWidth);
                    float posY = float(y * tileHeight);
                    cafes.emplace_back(cafeTexture, sf::Vector2f(posX, posY));
                    continue; 
            }
            
            // Enemies
            if(rawID == 292)
            {
                // Determine tile position as with other tiles
                int x = i % mapWidth;
                int y = i / mapWidth;
                float posX = float(x * tileWidth);
                float posY = float(y * tileHeight);

                // Create an enemy at this position
                enemies.push_back(Enemy(enemyTexture, {posX, posY}) );
            }
            // Detectar puerta
            if (rawID == 290 || rawID == 291)
            {
                tile.setTextureRect(sf::IntRect({0, 0}, {46, 56}));

                int x = i % mapWidth;
                int y = i / mapWidth;

                float posX = float(x * tileWidth);
                float posY = float((y + 1) * tileHeight - 56);

                tile.setPosition({posX, posY});
                tiles.push_back(tile);

                if (rawID == 291) {
                    entradaPosition = { posX, posY };

                    if (!texEntrada.loadFromFile("../assets/tilesets/Closiong (46x56).png")) {
                        std::cerr << "No se pudo cargar EntradaAnim.png\n";
                    } else {
                        animEntrada = Animation(0.1f);
                        animEntrada.setSpriteSheet(texEntrada);
                        for (int i = 0; i < 4; ++i){
                            animEntrada.addFrame(sf::IntRect({i * 46, 0}, {46, 56}));
                        }
                        puertaEntrada.setAnimation(&animEntrada);
                        puertaEntrada.setPosition({posX, posY});
                    }
                    continue;
                }

                // Al cargar puerta de salida (rawID == 290)
                if (rawID == 290) {
                    salidaRects.push_back(sf::FloatRect(sf::Vector2f(posX, posY), sf::Vector2f(46.f, 56.f)));

                    if (!texSalida.loadFromFile("../assets/tilesets/Opening (46x56).png")) {
                        std::cerr << "No se pudo cargar SalidaAnim.png\n";
                    } else {
                        animSalida = Animation(0.1f);
                        animSalida.setSpriteSheet(texSalida);
                        for (int i = 0; i < 6; ++i)
                            animSalida.addFrame(sf::IntRect({i * 46, 0}, {46, 56}));

                        puertaSalida.setAnimation(&animSalida);
                        puertaSalida.setPosition({posX, posY});
                    }

                    continue;
                }
            }

            if (tilesetIndex >= int(tilesetTextures.size())) continue;
            const sf::Texture& tex = tilesetTextures[tilesetIndex];

            int texWidth = tex.getSize().x / tileWidth;
            int sx = (localID % texWidth) * tileWidth;
            int sy = (localID / texWidth) * tileHeight;

            tile.setTexture(tex);
            tile.setTextureRect(sf::IntRect({sx, sy}, {tileWidth, tileHeight}));

            int x = i % mapWidth;
            int y = i / mapWidth;
            tile.setPosition({float(x * tileWidth), float(y * tileHeight)});

            tiles.push_back(tile);

            // Solo los tiles de la primera capa (i.e. la primera que contenga sólidos) son sólidos
            if (solidGIDs.find(rawID - 1) != solidGIDs.end())
            {
                solidTiles.push_back(tile);
                solidRects.emplace_back(sf::Vector2f(float(x * tileWidth), float(y * tileHeight)), sf::Vector2f(float(tileWidth), float(tileHeight)));
            }
        }
    }

    std::cout << "TiledMap cargado con " << tiles.size() << " tiles y "
              << solidTiles.size() << " sólidos.\n";
}

bool TiledMap::isColliding(const sf::FloatRect& bounds) const
{
    for (const auto& solid : solidRects)
    {
        if (intersects(bounds, solid))
            return true;
    }
    return false;
}

bool TiledMap::isTouchingSalida(const sf::FloatRect& bounds) const
{
    for (const auto& rect : salidaRects)
    {
        if (intersects(bounds, rect)){
            return true;
        }
    }
    return false;
}

sf::Vector2f TiledMap::getEntradaPosition() const
{
    return entradaPosition;
}

sf::Vector2f TiledMap::getPixelSize() const
{
    return sf::Vector2f(static_cast<float>(mapWidth * tileWidth), static_cast<float>(mapHeight * tileHeight));
}

sf::Sound* killEnemySoundPtr = nullptr;

void TiledMap::setKillEnemySound(sf::Sound* sound) {
    killEnemySoundPtr = sound;
}

sf::Sound* deathSoundPtr = nullptr;

void TiledMap::setDeathSound(sf::Sound* sound) {
    deathSoundPtr = sound;
}

bool TiledMap::update(float dt) {
    puertaEntrada.update(dt);
    puertaSalida.update(dt);

    //update for enemies:
    for(auto &enemy : enemies)
    {
        enemy.update(dt, this, playerPtr, killEnemySoundPtr);

        if (playerPtr) // Only if we have a valid pointer
        {
            sf::FloatRect eBounds = enemy.getBounds();
            sf::FloatRect pBounds = playerPtr->getBounds();
            

            if (intersects(eBounds, pBounds))
            {
                // Check if collision from above
                float playerBottom = pBounds.position.y + pBounds.position.x;
                float enemyTop     = eBounds.position.y + 5.f;
                bool  fromAbove    = (playerBottom < enemyTop);

                enemy.onPlayerCollision(fromAbove);

                if(fromAbove)
                {
                    std::cout << "Enemy killed by player!" << std::endl;
                }
                else
                {
                    std::cout << "Game Over!" << std::endl;
                    if (deathSoundPtr) deathSoundPtr->play();
                    // playerPtr->setPosition({-9999.f, -9999.f}); this use to sent the player off-screen, but we don't want that anymore
                    return true; // this indicates the player has been killed by an enemy
                }
            }
        }
    }
    return false; // No player killed by enemy
}

void TiledMap::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (const auto& tile : tiles)
        target.draw(tile, states);

    target.draw(puertaEntrada, states);
    target.draw(puertaSalida, states);
    
    // Draw all cafés
    for (const auto& cafe : cafes) {
        cafe.render(const_cast<sf::RenderWindow&>(
                      static_cast<const sf::RenderWindow&>(target)
                  ));
    }
    //draw enemies 
    for (auto &enemy : const_cast<std::vector<Enemy>&>(enemies))
        enemy.render(const_cast<sf::RenderWindow&>(
            static_cast<const sf::RenderWindow&>(target)
        ));
}

AnimatedDoor& TiledMap::getPuertaEntrada() {
    return puertaEntrada;
}

AnimatedDoor& TiledMap::getPuertaSalida() {
    return puertaSalida;
}
