//
// Created by Эвелина on 09.12.2025.
//

#ifndef PACMAN_GAME_H
#define PACMAN_GAME_H

#include <SFML/Graphics.hpp>
#include "GameMap.h"
#include "Pacman.h"
#include "Ghost.h"
#include <iostream>

using namespace std;

class Game {
private:
    GameMap gameMap;
    Pacman pacman;
    Ghost ghost1;
    Ghost ghost2;
    sf::RenderWindow window;
    sf::Font font;
    sf::Text gameOverText;
    sf::Text winText;
    sf::Clock gameOverClock;
    bool gameRunning;
    bool showGameOver;
    bool showWin;
    void initializeGhosts();
    void initializeText();
    void handleEvents();
    void update();
    void render();
public:
    Game();
    void run();
};


#endif //PACMAN_GAME_H