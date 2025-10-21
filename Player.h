#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"

class Player : public Entity {
private:
    float movementSpeed;
    sf::Vector2f windowBounds;
    int score;
    int lives;
    sf::Texture texture;

public:
    Player(float startX, float startY, sf::Vector2f windowSize);
    ~Player() override = default;

    void update() override;
    std::string getType() const override { return "Player"; }

    void moveLeft();
    void moveRight();
    void increaseScore(int points);
    void loseLife();
    
    int getScore() const;
    int getLives() const;
    sf::FloatRect getBounds() const override;

    void handleInput();

private:
    void initializeTexture();
};

#endif