//
// Created by Эвелина on 08.12.2025.
//

#ifndef PACMAN_GHOST_H
#define PACMAN_GHOST_H

#include <SFML/Graphics.hpp>
#include "GameMap.h"
#include "IGhost.h"
#include <vector>
#include <string>

using namespace std;

class Ghost : public IGhost {
private:
    int X, Y;
    int currentPoint;
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Clock clock;
    vector<pair<int, int>> Way;
    float moveTimer;
    float moveStop;
    void updateSpritePosition();
public:
    Ghost(int startX, int startY, const string& textures);
    void draw(sf::RenderWindow& window) override;
    void update(GameMap& map, int pacmanX, int pacmanY) override;
    bool chekPacman(int pacmanX, int pacmanY) override;
    void setWay(vector<pair<int, int>>& Way) override;
    int getX() override;
    int getY() override;
};

#endif //PACMAN_GHOST_H