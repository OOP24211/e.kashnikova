//
// Created by Эвелина on 09.12.2025.
//

#include "Game.h"

Game::Game():
    pacman(1, 1),
    ghost1(7, 9, "Blinky.png"),
    ghost2(9, 5, "Blinky.png"),
    gameRunning(true),
    showGameOver(false),
    showWin(false) {

    window.create(
        sf::VideoMode(
            gameMap.getPixelWidth(),
            gameMap.getPixelHeight()
        ),
        "Pacman"
    );

    initializeGhosts();
    initializeText();
}

void Game::initializeGhosts() {
    vector<pair<int, int>> way1 = {
        {7, 9}, {8, 9}, {9, 9}, {9, 10},
        {9, 11}, {9, 12}, {8, 12}, {7, 12},
        {7, 11}, {7, 10}, {7, 9}
    };

    vector<pair<int, int>> way2 = {
        {1, 5}, {2, 5}, {3, 5}, {3, 4}, {3, 3},
        {4, 3}, {5, 3}, {5, 4}, {5, 5}, {6, 5},
        {7, 5}, {7, 4}, {7, 3}, {8, 3}, {9, 3},
        {9, 4}, {9, 5}, {10, 5}, {11, 5}, {11, 4},
        {11, 3}, {12, 3}, {13, 3}, {13, 4}, {13, 5},
        {14, 5}, {15, 5}, {14, 5}, {13, 5}, {13, 4},
        {13, 3}, {12, 3}, {11, 3}, {11, 4}, {11, 5},
        {10, 5}, {9, 5}, {9, 4}, {9, 3}, {8, 3},
        {7, 3}, {7, 4}, {7, 5}, {6, 5}, {5, 5},
        {5, 4}, {5, 3}, {4, 3}, {3, 3}, {3, 4},
        {3, 5}, {2, 5}, {1, 5},
    };

    ghost1.setWay(way1);
    ghost2.setWay(way2);
}

void Game::initializeText() {
    font.loadFromFile("C:/Windows/Fonts/arial.ttf");

    gameOverText.setFont(font);
    gameOverText.setString("GAME OVER");
    gameOverText.setCharacterSize(48);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setPosition(gameMap.getPixelWidth() / 2 - 130, gameMap.getPixelHeight() / 2 - 24);

    winText.setFont(font);
    winText.setString("YOU WIN!");
    winText.setCharacterSize(48);
    winText.setFillColor(sf::Color::Green);
    winText.setPosition(gameMap.getPixelWidth() / 2 - 95, gameMap.getPixelHeight() / 2 - 24);
}

void Game::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        if (gameRunning) {
            pacman.handleEvent(event, gameMap);
        }
    }
}

void Game::update() {
    if (!gameRunning) {
        if (gameOverClock.getElapsedTime().asSeconds() >= 3.0f) {
            window.close();
        }
        return;
    }

    ghost1.update(gameMap, pacman.getX(), pacman.getY());
    ghost2.update(gameMap, pacman.getX(), pacman.getY());

    if (ghost1.chekPacman(pacman.getX(), pacman.getY()) ||
        ghost2.chekPacman(pacman.getX(), pacman.getY())) {
        std::cout << "         GAME OVER!" << std::endl;
        gameRunning = false;
        showGameOver = true;
        gameOverClock.restart();
    }

    if (!gameMap.chekDot()) {
        std::cout << "         WIN!" << std::endl;
        gameRunning = false;
        showWin = true;
        gameOverClock.restart();
    }
}

void Game::render() {
    window.clear(sf::Color::Black);

    gameMap.draw(window);
    pacman.draw(window);
    ghost1.draw(window);
    ghost2.draw(window);

    if (showGameOver && font.getInfo().family != "") {
        window.draw(gameOverText);
    } else if (showWin && font.getInfo().family != "") {
        window.draw(winText);
    }

    window.display();
}

void Game::run() {
    while (window.isOpen()) {
        handleEvents();
        update();
        render();
    }
}