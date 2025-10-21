#include "Enemy.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

Enemy::Enemy(float startX, float startY, int pointValue, float speed, int damage)
    : Entity(startX, startY, 10), movementSpeed(speed), 
      direction{0.0f, 1.0f}, pointValue(pointValue), shootCooldown(1.0f), 
      bulletDamage(damage), bulletSpeed(8.0f) {
    
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    initializeTexture();
}

void Enemy::initializeTexture() {
    if (!texture.loadFromFile("enemy.png")) {
        std::cerr << "Error: Could not load enemy.png. Using default shape." << std::endl;
        shape = new sf::CircleShape(10.0f);
        shape->setPosition(position);
        shape->setFillColor(sf::Color::Magenta);
        shape->setOrigin(sf::Vector2f{10.0f, 10.0f});
    } else {
        shape = new sf::RectangleShape(sf::Vector2f(30.0f, 30.0f));
        shape->setTexture(&texture);
        shape->setPosition(position);
        shape->setOrigin(sf::Vector2f{15.0f, 15.0f});
    }
}

void Enemy::update() {
    move();
    if (shape) {
        shape->setPosition(position);
    }
}

void Enemy::move() {
    position += direction * movementSpeed;
}

bool Enemy::canShoot() {
    if (shootClock.getElapsedTime().asSeconds() > shootCooldown) {
        shootClock.restart();
        return true;
    }
    return false;
}

bool Enemy::isOffScreen() const {
    return position.y > 650.0f;
}

int Enemy::getPointValue() const {
    return pointValue;
}

int Enemy::getBulletDamage() const {
    return bulletDamage;
}

float Enemy::getBulletSpeed() const {
    return bulletSpeed;
}

void Enemy::setBulletSpeed(float speed) {
    bulletSpeed = speed;
}

sf::FloatRect Enemy::getBounds() const {
    if (shape) {
        return shape->getGlobalBounds();
    }
    return sf::FloatRect();
}

void Enemy::setSpeed(float newSpeed) {
    movementSpeed = newSpeed;
}

void Enemy::setBulletDamage(int damage) {
    bulletDamage = damage;
}