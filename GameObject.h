#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SFML/Graphics.hpp>
#include <string>
#include <memory>

/**
 * @brief Abstract base class for all game objects
 * 
 * This class defines the interface that all game objects must implement.
 * It uses pure virtual functions to enforce proper implementation in derived classes.
 */
class GameObject {
protected:
    sf::Vector2f position;
    bool active;

public:
    GameObject(float x, float y);
    virtual ~GameObject() = default;

    // Pure virtual functions - must be implemented by derived classes
    virtual void update() = 0;
    virtual void render(sf::RenderWindow& window) const = 0;
    virtual std::string getState() const = 0;
    virtual void setState(const std::string& state) = 0;

    // Virtual functions with default implementation
    virtual sf::FloatRect getBounds() const;
    virtual bool isActive() const;
    virtual void setActive(bool active);
    virtual sf::Vector2f getPosition() const;
    virtual void setPosition(const sf::Vector2f& newPosition);

    // Static type identifier
    virtual std::string getType() const = 0;
};

#endif