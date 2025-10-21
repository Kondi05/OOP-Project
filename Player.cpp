#include "Player.h"
#include <SFML/Window/Keyboard.hpp>
#include <sstream>
#include <iostream>

Player::Player(float startX, float startY, sf::Vector2f windowSize)
    : Entity(startX, startY, 100), movementSpeed(5.0f), 
      windowBounds(windowSize), score(0), lives(3) {
    initializeTexture();
}

void Player::initializeTexture() {
    // Try to load the player texture
    if (!texture.loadFromFile("player.png")) {
        std::cerr << "Error: Could not load player.png. Using default shape." << std::endl;
        // Fallback to rectangle if texture fails
        shape = new sf::RectangleShape(sf::Vector2f(50.0f, 20.0f));
        shape->setPosition(position);
        shape->setFillColor(sf::Color::Green);
        shape->setOrigin(sf::Vector2f{25.0f, 10.0f}); // Fixed
    } else {
        // Use texture for player
        shape = new sf::RectangleShape(sf::Vector2f(60.0f, 40.0f));
        shape->setTexture(&texture);
        shape->setPosition(position);
        shape->setOrigin(sf::Vector2f{30.0f, 20.0f}); // Fixed
    }
}

void Player::update() {
    handleInput();
    if (shape) {
        shape->setPosition(position);
    }
}

void Player::moveLeft() {
    if (position.x - 30.0f > 0) {
        position.x -= movementSpeed;
    }
}

void Player::moveRight() {
    if (position.x + 30.0f < windowBounds.x) {
        position.x += movementSpeed;
    }
}

void Player::increaseScore(int points) {
    score += points;
}

void Player::loseLife() {
    if (lives > 0) {
        lives--;
    }
}

int Player::getScore() const {
    return score;
}

int Player::getLives() const {
    return lives;
}

sf::FloatRect Player::getBounds() const {
    if (shape) {
        return shape->getGlobalBounds();
    }
    return sf::FloatRect();
}

void Player::handleInput() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
        moveLeft();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
        moveRight();
    }
}