//
// Created by Эвелина on 05.12.2025.
//

#include "Pacman.h"
#include "GameMap.h"

Pacman::Pacman(int startX, int startY) {
    X = startX;
    Y = startY;

    shape.setRadius(12.0f);
    shape.setFillColor(sf::Color::Yellow);

    float pixelX = X * SIZE + SIZE / 2 - 12;
    float pixelY = Y * SIZE + SIZE / 2 - 12;
    shape.setPosition(pixelX, pixelY);
}

void Pacman::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

void Pacman::handleEvent(const sf::Event& event, GameMap& gameMap) {
    if (event.type == sf::Event::KeyPressed) {
        handleKeyPress(event.key.code, gameMap);
    }
}

void Pacman::handleKeyPress(sf::Keyboard::Key key, GameMap& gameMap) {
    switch (key) {
        case sf::Keyboard::Right:
            if (tryMove(X + 1, Y, gameMap)) {
                gameMap.removeDot(X,Y);
                X++;
            }
            break;
        case sf::Keyboard::Down:
            if (tryMove(X, Y + 1, gameMap)) {
                gameMap.removeDot(X,Y);
                Y++;
            }
            break;
        case sf::Keyboard::Left:
            if (tryMove(X - 1, Y, gameMap)) {
                gameMap.removeDot(X,Y);
                X--;
            }
            break;
        case sf::Keyboard::Up:
            if (tryMove(X, Y - 1, gameMap)) {
                gameMap.removeDot(X,Y);
                Y--;
            }
            break;
        default:
            break;
    }


        float pixelX = X * SIZE + SIZE / 2 - 12;
        float pixelY = Y * SIZE + SIZE / 2 - 12;
        shape.setPosition(pixelX, pixelY);
    }


bool Pacman::tryMove(int newX, int newY, GameMap& gameMap) {
    if (gameMap.getSymbol(newX, newY) == '#') {
        return false;
    }
    return true;
}

int Pacman::getX() {return X;}
int Pacman::getY() {return Y;}