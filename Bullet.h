#ifndef BULLET_H
#define BULLET_H

#include "Entity.h"

class Bullet : public Entity {
private:
    sf::Vector2f direction;
    float movementSpeed;
    bool isPlayerBullet;

public:
    Bullet(float startX, float startY, bool isPlayerBullet = true, float speed = 8.0f);
    ~Bullet() override = default;

    void update() override;
    std::string getType() const override { return "Bullet"; }

    bool isFromPlayer() const;
    void setSpeed(float speed);
    
    sf::FloatRect getBounds() const override;

private:
    void initializeShape();
};

#endif