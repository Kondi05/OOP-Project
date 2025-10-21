#include "Entity.h"
#include <sstream>

Entity::Entity(float x, float y, int health) 
    : GameObject(x, y), shape(nullptr), health(health), maxHealth(health) {}

Entity::~Entity() {
    delete shape;
}

void Entity::render(sf::RenderWindow& window) const {
    if (shape && active) {
        window.draw(*shape);
    }
}

std::string Entity::getState() const {
    std::stringstream ss;
    ss << "Position: (" << position.x << ", " << position.y << "), "
       << "Health: " << health << "/" << maxHealth << ", "
       << "Active: " << (active ? "true" : "false");
    return ss.str();
}

void Entity::setState(const std::string& state) {
    // Simplified state parsing
    // In a full implementation, you would parse the string and set properties
    (void)state;
}

void Entity::takeDamage(int damage) {
    health -= damage;
    if (health < 0) health = 0;
    updateColorBasedOnHealth();
}

int Entity::getHealth() const {
    return health;
}

int Entity::getMaxHealth() const {
    return maxHealth;
}

bool Entity::isAlive() const {
    return health > 0;
}

void Entity::setHealth(int newHealth) {
    health = newHealth;
    if (health > maxHealth) health = maxHealth;
    if (health < 0) health = 0;
    updateColorBasedOnHealth();
}

sf::Color Entity::getColor() const {
    return shape ? shape->getFillColor() : sf::Color::White;
}

void Entity::setColor(const sf::Color& color) {
    if (shape) {
        shape->setFillColor(color);
    }
}

void Entity::updateColorBasedOnHealth() {
    if (!shape) return;
    
    float healthRatio = static_cast<float>(health) / maxHealth;
    if (healthRatio > 0.7f) {
        shape->setFillColor(sf::Color::Green);
    } else if (healthRatio > 0.3f) {
        shape->setFillColor(sf::Color::Yellow);
    } else {
        shape->setFillColor(sf::Color::Red);
    }
}