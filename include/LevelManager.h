#pragma once
#include <vector>
#include <string>
#include <memory>
#include <SFML/Graphics.hpp>
#include "TiledMap.h"

class LevelManager
{
public:
    LevelManager(const std::vector<std::string>& mapFiles, const std::vector<std::string>& tilesetPaths);

    void loadLevel(int index);
    void loadNextLevel();
    void reloadCurrentLevel();

    void draw(sf::RenderTarget& target);

    TiledMap* getCurrentMap();

    int getCurrentLevelIndex() const;

private:
    std::vector<std::string> mapFiles;
    std::vector<std::string> tilesetPaths;

    std::unique_ptr<TiledMap> currentMap;
    int currentLevelIndex = 0;
};