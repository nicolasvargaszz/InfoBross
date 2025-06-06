#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <unordered_set>

class TiledMap : public sf::Drawable
{
public:
    // Pass paths to the JSON map file and the spritesheet
    TiledMap(const std::string& jsonMapPath, const std::string& texturePath);

    // Add this to obtain collisions
    const std::vector<sf::Sprite>& getSolidTiles() const { return solidTiles; }
    // Verify collision 
    bool isColliding(const sf::FloatRect& bounds) const;

    sf::Vector2f getPixelSize() const;
    
private:
    int mapWidth;   
    int mapHeight;
    int tileWidth;
    int tileHeight;

    std::vector<sf::FloatRect> solidRects;
    sf::Texture mapTexture;
    std::vector<sf::Sprite> tiles;
    
    std::vector<sf::Sprite> solidTiles;
    std::unordered_set<int> solidGIDs = { /* IDs solid */ 
        20,21,22,24,26,27,29,30,32,33,35,
        39,41,43,45,46,48,49,51,52,54,55,
        58,59,60,62,83,84,86,87,89,90,92,
        93,96,97,98,100,102,103,105,106,
        108,109,111,112
    };

    // Draw override (required by sf::Drawable)
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};