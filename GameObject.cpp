#include "GameObject.h"

GameObject::GameObject(float x, float y) 
    : position(x, y), active(true) {}

sf::FloatRect GameObject::getBounds() const {
    return sf::FloatRect(position, sf::Vector2f(1, 1));
}

bool GameObject::isActive() const {
    return active;
}

void GameObject::setActive(bool active) {
    this->active = active;
}

sf::Vector2f GameObject::getPosition() const {
    return position;
}

void GameObject::setPosition(const sf::Vector2f& newPosition) {
    position = newPosition;
}