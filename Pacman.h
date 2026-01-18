//
// Created by Эвелина on 05.12.2025.
//

#ifndef PACMAN_PACMAN_H
#define PACMAN_PACMAN_H

#include <SFML/Graphics.hpp>
#include "GameMap.h"

class Pacman {
private:
    sf::CircleShape shape;
    int X, Y;
    bool tryMove(int newX, int newY, GameMap& gameMap);
    void handleKeyPress(sf::Keyboard::Key key, GameMap& gameMap);

public:
    Pacman(int startX, int startY);
    void handleEvent(const sf::Event& event, GameMap& gameMap);
    void draw(sf::RenderWindow& window);
    int getX();
    int getY();

};


#endif //PACMAN_PACMAN_H