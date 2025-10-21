#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.h"

class Enemy : public Entity {
private:
    float movementSpeed;
    sf::Vector2f direction;
    int pointValue;
    sf::Clock shootClock;
    float shootCooldown;
    int bulletDamage;
    float bulletSpeed;
    sf::Texture texture;

public:
    Enemy(float startX, float startY, int pointValue = 100, float speed = 1.0f, int damage = 10);
    ~Enemy() override = default;

    void update() override;
    std::string getType() const override { return "Enemy"; }

    void move();
    bool canShoot();
    int getPointValue() const;
    int getBulletDamage() const;
    float getBulletSpeed() const;
    void setBulletSpeed(float speed);
    
    sf::FloatRect getBounds() const override;

    void setSpeed(float newSpeed);
    void setBulletDamage(int damage);
    bool isOffScreen() const;

private:
    void initializeTexture();
};

#endif