#include "../include/TiledMap.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <unordered_set>

bool intersects(const sf::FloatRect& a, const sf::FloatRect& b)
{
    return a.position.x < b.position.x + b.size.x &&
           a.position.x + a.size.x > b.position.x &&
           a.position.y < b.position.y + b.size.y &&
           a.position.y + a.size.y > b.position.y;
}

TiledMap::TiledMap(const std::string& jsonMapPath, const std::string& texturePath)
{
    // 1) Load the texture (your spritesheet)
    if (!mapTexture.loadFromFile(texturePath))
    {
        std::cerr << "Failed to load " << texturePath << std::endl;
        return;
    }
    std::cout << "Loaded " << texturePath << " OK!\n";

    // 2) Open and parse the JSON map file
    std::ifstream file(jsonMapPath);
    if (!file.is_open())
    {
        std::cerr << "Could not open " << jsonMapPath << std::endl;
        return;
    }
    std::cout << "Opened " << jsonMapPath << " OK!\n";

    nlohmann::json mapData;
    file >> mapData;

    // 3) Get basic map info
    tileWidth  = mapData["tilewidth"];
    tileHeight = mapData["tileheight"];
    mapWidth   = mapData["width"];
    mapHeight  = mapData["height"];

    // 4) Assume the tile indices are in layers[0]["data"]
    if (!mapData.contains("layers") || mapData["layers"].empty())
    {
        std::cerr << "No layers found in the JSON map\n";
        return;
    }

    auto dataArray = mapData["layers"][0]["data"];
    if (dataArray.empty())
    {
        std::cout << "Layer data is empty\n";
        return;
    }

    // 5) Generate sprites for each non-zero tile ID
    for (int i = 0; i < static_cast<int>(dataArray.size()); ++i)
    {
        int rawID = dataArray[i];
        if (rawID > 0) // Zero means no tile
        {
            int tileID = rawID - 1; // Convert Tiled's 1-based index to 0-based
            // The number of tiles across the spritesheet
            int sheetCols = mapTexture.getSize().x / tileWidth;

            // Sub-rectangle within spritesheet
            int sx = (tileID % sheetCols) * tileWidth;
            int sy = (tileID / sheetCols) * tileHeight;

            sf::Sprite tile(mapTexture);
            tile.setTextureRect(sf::IntRect(
                sf::Vector2i(sx, sy),
                sf::Vector2i(tileWidth, tileHeight)));

            // Position in the world
            int x = i % mapWidth;
            int y = i / mapWidth;
            tile.setPosition(sf::Vector2f(
                float(x * tileWidth),
                float(y * tileHeight)));

            tiles.push_back(tile);

            if (solidGIDs.find(tileID) != solidGIDs.end())
            {
                solidTiles.push_back(tile);

                // Agrega el rectángulo sólido
                sf::FloatRect rect;
                rect.position = sf::Vector2f(float(x * tileWidth), float(y * tileHeight));
                rect.size = sf::Vector2f(float(tileWidth), float(tileHeight));
                solidRects.push_back(rect);
            }
        }
    }

    std::cout << "TiledMap loaded " << tiles.size() << " tiles, "
              << solidTiles.size() << " solid tiles.\n";
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

// 6) Implement the draw method
void TiledMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (auto& tile : tiles)
        target.draw(tile, states);
}

sf::Vector2f TiledMap::getPixelSize() const {
        return sf::Vector2f(mapWidth * tileWidth, mapHeight * tileHeight);
}