//
// Created by Эвелина on 08.12.2025.
//

#ifndef PACMAN_IGHOST_H
#define PACMAN_IGHOST_H

#include <SFML/Graphics.hpp>
#include "GameMap.h"
#include <vector>

class IGhost {
public:
    virtual ~IGhost() = default;
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual void update(GameMap& map, int pacmanX, int pacmanY) = 0;
    virtual bool chekPacman(int pacmanX, int pacmanY) = 0;
    virtual int getX() = 0;
    virtual int getY() = 0;
    virtual void setWay(vector<pair<int, int>>& Way) = 0;
};


#endif //PACMAN_IGHOST_H