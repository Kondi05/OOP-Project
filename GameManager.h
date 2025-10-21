#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <optional>
#include <algorithm>
#include <vector>
#include <memory>
#include <string>
#include <iostream>
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include "FileHandler.h"

/**
 * @brief Main game controller managing game state, objects, and logic
 * 
 * Demonstrates proper memory management with smart pointers and error handling
 */
class GameManager {
private:
    sf::RenderWindow window;
    std::unique_ptr<Player> player;
    std::vector<std::unique_ptr<Bullet>> bullets;
    std::vector<std::unique_ptr<Enemy>> enemies;
    FileHandler fileHandler;
    
    bool gameRunning;
    bool gamePaused;
    sf::Clock gameClock;
    sf::Clock shootClock;
    float playerShootCooldown;

    // Game progression system
    int currentLevel;
    int highScore;
    // Removed: sf::Font font; - Causing the constructor error

    // Game state management
    void initializeGame();
    void spawnEnemies();
    void handleCollisions();
    void cleanupInactiveObjects();
    void displayMenu();
    void handleMenuInput();
    void spawnWave();
    void nextLevel();
    void renderUI(); // New method for UI rendering
    void saveHighScore();
    void loadHighScore();

public:
    GameManager();
    ~GameManager() = default;

    // Main game loop
    void run();
    
    // Game state operations
    void saveGame();
    void loadGame();
    void resetGame();
    
    // Input validation and error handling
    bool validateGameState() const;
    void handleRuntimeErrors();

private:
    void processInput();
    void update();
    void render();
    void displayGameOver();
};

#endif