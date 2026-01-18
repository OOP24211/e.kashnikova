//
// Created by Эвелина on 04.12.2025.
//

#ifndef PACMAN_GAMEMAP_H
#define PACMAN_GAMEMAP_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

const int SIZE = 32;
using namespace std;

class GameMap {
private:
    vector<string> gameMap;
    int width;
    int height;

    void drawWall(sf::RenderWindow& window, sf::Vector2f position);
    void drawDot(sf::RenderWindow& window, sf::Vector2f position);

public:
    GameMap();
    void draw(sf::RenderWindow& window);
    char getSymbol(int X, int Y) const;
    void removeDot(int X, int Y);
    bool chekDot();
    int getWidth();
    int getHeight();
    int getPixelWidth();
    int getPixelHeight();
};


#endif //PACMAN_GAMEMAP_H