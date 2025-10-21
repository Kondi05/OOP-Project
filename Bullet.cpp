#include "Bullet.h"

Bullet::Bullet(float startX, float startY, bool isPlayerBullet, float speed)
    : Entity(startX, startY, 1), movementSpeed(speed), isPlayerBullet(isPlayerBullet) {
    
    direction = isPlayerBullet ? sf::Vector2f{0.0f, -1.0f} : sf::Vector2f{0.0f, 1.0f};
    initializeShape();
}

void Bullet::initializeShape() {
    shape = new sf::RectangleShape(sf::Vector2f(2.0f, 8.0f));
    shape->setPosition(position);
    shape->setFillColor(isPlayerBullet ? sf::Color::White : sf::Color::Red);
    shape->setOrigin(sf::Vector2f{1.0f, 4.0f});
}

void Bullet::update() {
    if (active) {
        position += direction * movementSpeed;
        
        if (shape) {
            shape->setPosition(position);
        }
        
        if (position.y < -10.0f || position.y > 610.0f) {
            active = false;
        }
    }
}

bool Bullet::isFromPlayer() const {
    return isPlayerBullet;
}

void Bullet::setSpeed(float speed) {
    movementSpeed = speed;
}

sf::FloatRect Bullet::getBounds() const {
    if (shape) {
        return shape->getGlobalBounds();
    }
    return sf::FloatRect();
}