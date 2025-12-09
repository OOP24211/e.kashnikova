//
// Created by Эвелина on 08.12.2025.
//

#include "Ghost.h"
#include <iostream>
#include <algorithm>

Ghost::Ghost(int startX, int startY, const string& textures) {
    X = startX;
    Y = startY;
    currentPoint = 0;
    moveTimer = 0.0f;
    moveStop = 0.15f;  //пауза между перемещениями

    if (!texture.loadFromFile(textures)) {
        std::cerr << "Error loading ghost texture: " << textures << std::endl;
    }
    sprite.setTexture(texture);

    float scale = SIZE / max(texture.getSize().x, texture.getSize().y) * 0.8f;
    sprite.setScale(scale, scale);
    updateSpritePosition();
}

void Ghost::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

void Ghost::update(GameMap& map, int pacmanX, int pacmanY) {
    if (Way.empty()) {
        return;
    }
    float time = clock.restart().asSeconds();
    moveTimer += time;

    if (moveTimer >= moveStop) {
        moveTimer = 0.0f;

        int nextX = Way[currentPoint].first;
        int nextY = Way[currentPoint].second;

        X = nextX;
        Y = nextY;

        currentPoint = (currentPoint + 1) % Way.size();
        updateSpritePosition();
    }
}

void Ghost::updateSpritePosition() {
    float pixelX = X * SIZE + (SIZE - sprite.getGlobalBounds().width) / 2;
    float pixelY = Y * SIZE + (SIZE - sprite.getGlobalBounds().height) / 2;
    sprite.setPosition(pixelX, pixelY);
}

bool Ghost::chekPacman(int pacmanX, int pacmanY) {
    return (X == pacmanX && Y == pacmanY);
}

void Ghost::setWay(vector<pair<int, int>>& newWay) {
    Way = newWay;
    currentPoint = 0;
    if (!Way.empty()) {
        X = Way[0].first;
        Y = Way[0].second;
        updateSpritePosition();
    }
}

int Ghost::getX() { return X; }
int Ghost::getY() { return Y; }