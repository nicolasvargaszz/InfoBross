#include "../include/LevelManager.h"
#include <iostream>

LevelManager::LevelManager(const std::vector<std::string>& mapFiles, const std::vector<std::string>& tilesetPaths)
    : mapFiles(mapFiles), tilesetPaths(tilesetPaths)
{
    if (!mapFiles.empty())
        loadLevel(0);
}

void LevelManager::loadLevel(int index)
{
    if (index < 0 || index >= static_cast<int>(mapFiles.size()))
    {
        std::cerr << "LevelManager: índice de nivel fuera de rango: " << index << std::endl;
        return;
    }

    currentLevelIndex = index;
    currentMap = std::make_unique<TiledMap>(mapFiles[currentLevelIndex], tilesetPaths);

    std::cout << "Cargado nivel " << currentLevelIndex << " desde: " << mapFiles[currentLevelIndex] << std::endl;
}

void LevelManager::loadNextLevel()
{
    int nextIndex = currentLevelIndex + 1;
    if (nextIndex >= static_cast<int>(mapFiles.size()))
    {
        std::cout << "No hay más niveles, reiniciando al primero." << std::endl;
        nextIndex = 0;
    }
    loadLevel(nextIndex);
}

void LevelManager::reloadCurrentLevel()
{
    loadLevel(currentLevelIndex);
}

void LevelManager::draw(sf::RenderTarget& target)
{
    if (currentMap)
        currentMap->draw(target, sf::RenderStates::Default);
}

TiledMap* LevelManager::getCurrentMap()
{
    return currentMap.get();
}

int LevelManager::getCurrentLevelIndex() const
{
    return currentLevelIndex;
}