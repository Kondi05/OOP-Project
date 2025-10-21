#ifndef ENTITY_H
#define ENTITY_H

#include "GameObject.h"
#include <SFML/Graphics.hpp>

/*
    abstract class for entities
    Adds rendering and health system to the GameObject class.
    3-level inheritance: GameObject -> Entity -> Bullet.cpp etc
 */
class Entity : public GameObject {
protected:
    sf::Shape* shape;
    int health;
    int maxHealth;

public:
    Entity(float x, float y, int health);
    virtual ~Entity();

    // Pure virtual functions from GameObject
    void update() override = 0;
    void render(sf::RenderWindow& window) const override;
    std::string getState() const override;
    void setState(const std::string& state) override;

    // Entity-specific functionality
    virtual void takeDamage(int damage);
    int getHealth() const;
    int getMaxHealth() const;
    bool isAlive() const;
    
    // Getters and setters with proper encapsulation
    void setHealth(int newHealth);
    sf::Color getColor() const;
    void setColor(const sf::Color& color);

protected:
    void updateColorBasedOnHealth();
};

#endif