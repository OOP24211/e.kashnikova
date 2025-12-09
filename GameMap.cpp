//
// Created by Эвелина on 04.12.2025.
//

#include "GameMap.h"

void GameMap::drawWall(sf::RenderWindow& window, sf::Vector2f position) {
    sf::RectangleShape wall(sf::Vector2f(SIZE, SIZE));
    wall.setFillColor(sf::Color::Blue);
    wall.setPosition(position);
    window.draw(wall);
}

void GameMap::drawDot(sf::RenderWindow& window, sf::Vector2f position) {
    float radius = SIZE / 10;
    sf::CircleShape dot(radius);
    dot.setFillColor(sf::Color::White);

    float centerX = position.x + SIZE / 2 - radius;
    float centerY = position.y + SIZE / 2 - radius;
    dot.setPosition(centerX, centerY);
    window.draw(dot);
}

GameMap::GameMap() {
    gameMap = {
        "#################",   //map[0]
        "#...............#",   //map[1]
        "#.###.#####.###.#",
        "#.#...#...#...#.#",
        "#.#.#.#.#.#.#.#.#",
        "#...#...#...#...#",
        "#.###.#####.###.#",
        "#...#.......#...#",
        "#.#.#.#####.#.#.#",
        "#.#...#...#...#.#",
        "#...#...#...#...#",
        "#.#####.#.#####.#",
        "#...............#",
        "#################"
    };
    height = gameMap.size();
    if (height > 0){ width = gameMap[0].size();}
}

void GameMap::draw(sf::RenderWindow& window){
    for (int line = 0; line < height; line++) {
        for (int col = 0; col < width; col++) {
            char symbol = gameMap[line][col];
            float pixelY = line * SIZE;
            float pixelX = col * SIZE;
            sf::Vector2f position(pixelX, pixelY);  //двумерный вектор с float, класс из SFML

            switch (symbol) {
                case '#':
                    drawWall(window, position);
                    break;
                case '.':
                    drawDot(window, position);
                    break;
                case ' ':
                    break;
            }
        }
    }
}

char GameMap::getSymbol(int X, int Y) const {
    if (Y >= 0 && Y < height &&
        X >= 0 && X < width) {
        return gameMap[Y][X];
    }
    return '#';
}

void GameMap::removeDot(int X, int Y) {
    if (Y >= 0 && Y < height &&
        X >= 0 && X < width) {
        if (gameMap[Y][X] == '.'){ gameMap[Y][X] = ' ';}
    }
}

bool GameMap::chekDot() {
    for (int line = 0; line < height; line++) {
        for (int col = 0; col < width; col++) {
            if (gameMap[line][col] == '.') {return true;}
        }
    }
    return false;
}

int GameMap::getWidth() { return width; }
int GameMap::getHeight() { return height; }
int GameMap::getPixelWidth() { return width * SIZE; }
int GameMap::getPixelHeight() { return height * SIZE; }
