#include "GameManager.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cmath>
#include <fstream>

GameManager::GameManager() 
    : window(sf::VideoMode({800, 600}), "Space Invaders"),
      gameRunning(true), gamePaused(false), playerShootCooldown(0.3f),
      currentLevel(1), highScore(0) {
    
    window.setFramerateLimit(60);
    window.setPosition(sf::Vector2i{500, 200});
    loadHighScore();
    initializeGame();
}

void GameManager::initializeGame() {
    try {
        player = std::make_unique<Player>(400.0f, 550.0f, sf::Vector2f{800.0f, 600.0f});
        spawnEnemies();
        std::cout << "Game initialized successfully" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error initializing game: " << e.what() << std::endl;
        gameRunning = false;
    }
}

void GameManager::spawnEnemies() {
    spawnWave();
}

void GameManager::spawnWave() {
     int enemiesPerRow = 4;
    int rows = 2;
    
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < enemiesPerRow; ++col) {
            float x = 100.0f + col * 80.0f;
            float y = -30.0f - row * 60.0f; // Back row starts higher up
            float enemySpeed = 0.5f + (currentLevel - 1) * 1.2f; // Increased speed scaling
            int bulletDamage = 15 + (currentLevel - 1) * 8; // Increased damage scaling
            float bulletSpeed = 2.0f + (currentLevel - 1) * 1.2f; // Bullet speed increases
            int pointValue = 50 + (currentLevel * 25);
            
            auto enemy = std::make_unique<Enemy>(x, y, pointValue, enemySpeed, bulletDamage);
            enemy->setBulletSpeed(bulletSpeed); // Set bullet speed for this enemy
            enemies.push_back(std::move(enemy));
        }
    }
    
    std::cout << "🔥 Wave " << currentLevel << " spawned! " 
              << "Enemies: 16 (8 front + 8 back)" 
              << ", Speed: " << (1.0f + (currentLevel - 1) * 1.2f)
              << ", Bullet Damage: " << (15 + (currentLevel - 1) * 8)
              << ", Bullet Speed: " << (8.0f + (currentLevel - 1) * 2.0f) << std::endl;
}

void GameManager::nextLevel() {
    currentLevel++;
    std::cout << "🚀 LEVEL " << currentLevel << "! Difficulty increased!" << std::endl;
}

void GameManager::run() {
    sf::Clock waveClock;
    sf::Clock levelClock;
    
    while (window.isOpen() && gameRunning) {
        processInput();
        
        if (!gamePaused) {
            update();
            
            // Spawn new wave only when all enemies are gone, with a 3 seconds delay 
            if (enemies.empty()) {
                if (waveClock.getElapsedTime().asSeconds() > 3.0f) {
                spawnWave();
                waveClock.restart();
                }
            }
            
            // Increase difficulty every 45 seconds
            if (levelClock.getElapsedTime().asSeconds() > 45.0f) {
                nextLevel();
                levelClock.restart();
            }
        }
        
        render();
        
        if (gamePaused) {
            displayMenu();
        }
        
        if (player->getLives() <= 0) {
            displayGameOver();
            saveHighScore();
            gameRunning = false;
        }
    }
}

void GameManager::processInput() {
    while (std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
        
        if (event->is<sf::Event::KeyPressed>()) {
            if (auto keyEvent = event->getIf<sf::Event::KeyPressed>()) {
                switch (keyEvent->code) {
                    case sf::Keyboard::Key::Escape:
                        gamePaused = !gamePaused;
                        break;
                    case sf::Keyboard::Key::Space:
                        if (!gamePaused && shootClock.getElapsedTime().asSeconds() > playerShootCooldown) {
                            auto bullet = std::make_unique<Bullet>(
                                player->getPosition().x, 
                                player->getPosition().y - 15.0f,
                                true
                            );
                            bullets.push_back(std::move(bullet));
                            shootClock.restart();
                        }
                        break;
                    case sf::Keyboard::Key::S:
                        if (gamePaused) saveGame();
                        break;
                    case sf::Keyboard::Key::L:
                        if (gamePaused) loadGame();
                        break;
                    case sf::Keyboard::Key::R:
                        if (gamePaused) resetGame();
                        break;
                    case sf::Keyboard::Key::Q:
                        if (gamePaused) window.close();
                        break;
                    default:
                        break;
                }
            }
        }
    }
}

void GameManager::update() {
    try {
        player->update();
        
        for (auto& bullet : bullets) {
            bullet->update();
        }
        
        // ALL enemies shoot continuously every second
        for (auto& enemy : enemies) {
            enemy->update();
            
            if (enemy->canShoot()) {
                auto bullet = std::make_unique<Bullet>(
                    enemy->getPosition().x,
                    enemy->getPosition().y + 20.0f,
                    false,
                    dynamic_cast<Enemy*>(enemy.get())->getBulletSpeed() // Use enemy's bullet speed
                );
                bullets.push_back(std::move(bullet));
            }
        }
        
        handleCollisions();
        cleanupInactiveObjects();
        
    } catch (const std::exception& e) {
        std::cerr << "Error in game update: " << e.what() << std::endl;
        handleRuntimeErrors();
    }
}

void GameManager::handleCollisions() {
    // Player bullets vs Enemies
    for (auto bulletIt = bullets.begin(); bulletIt != bullets.end(); ) {
        if ((*bulletIt)->isFromPlayer()) {
            for (auto enemyIt = enemies.begin(); enemyIt != enemies.end(); ) {
                sf::Vector2f bulletPos = (*bulletIt)->getPosition();
                sf::Vector2f enemyPos = (*enemyIt)->getPosition();
                
                float dx = bulletPos.x - enemyPos.x;
                float dy = bulletPos.y - enemyPos.y;
                float distance = std::sqrt(dx * dx + dy * dy);
                
                bool collision = (distance < 20.0f);
                
                if (collision) {
                    (*enemyIt)->takeDamage(10);
                    if (!(*enemyIt)->isAlive()) {
                        int points = dynamic_cast<Enemy*>((*enemyIt).get())->getPointValue();
                        player->increaseScore(points);
                        
                        if (player->getScore() > highScore) {
                            highScore = player->getScore();
                        }
                        
                        enemyIt = enemies.erase(enemyIt);
                    } else {
                        ++enemyIt;
                    }
                    (*bulletIt)->setActive(false);
                    break;
                } else {
                    ++enemyIt;
                }
            }
        }
        ++bulletIt;
    }
    
    // Enemy bullets vs Player (increasing damage based on level)
    for (auto& bullet : bullets) {
        if (!bullet->isFromPlayer()) {
            sf::Vector2f bulletPos = bullet->getPosition();
            sf::Vector2f playerPos = player->getPosition();
            
            float dx = bulletPos.x - playerPos.x;
            float dy = bulletPos.y - playerPos.y;
            float distance = std::sqrt(dx * dx + dy * dy);
            
            bool collision = (distance < 30.0f);
            
            if (collision) {
                // Find which enemy shot this bullet and get its damage
                int bulletDamage = 10; // Default damage
                for (auto& enemy : enemies) {
                    // Simple proximity check to find likely shooter
                    float enemyDist = std::sqrt(
                        std::pow(bulletPos.x - enemy->getPosition().x, 2) +
                        std::pow(bulletPos.y - enemy->getPosition().y, 2)
                    );
                    if (enemyDist < 100.0f) {
                        bulletDamage = dynamic_cast<Enemy*>(enemy.get())->getBulletDamage();
                        break;
                    }
                }
                
                player->takeDamage(bulletDamage);
                bullet->setActive(false);
                if (!player->isAlive()) {
                    player->loseLife();
                    if (player->getLives() > 0) {
                        player->setHealth(100);
                        player->setPosition(sf::Vector2f{400.0f, 550.0f});
                    }
                }
            }
        }
    }
    
    // Enemy vs Player collision (continuous damage)
    for (auto enemyIt = enemies.begin(); enemyIt != enemies.end(); ) {
        sf::Vector2f enemyPos = (*enemyIt)->getPosition();
        sf::Vector2f playerPos = player->getPosition();
        
        float dx = enemyPos.x - playerPos.x;
        float dy = enemyPos.y - playerPos.y;
        float distance = std::sqrt(dx * dx + dy * dy);
        
        bool collision = (distance < 35.0f);
        
        if (collision) {
            // Continuous collision damage - higher at higher levels
            int collisionDamage = 15 + (currentLevel - 1) * 3;
            player->takeDamage(collisionDamage);
            
            // Remove enemy after collision
            enemyIt = enemies.erase(enemyIt);
            
            if (!player->isAlive()) {
                player->loseLife();
                if (player->getLives() > 0) {
                    player->setHealth(100);
                    player->setPosition(sf::Vector2f{400.0f, 550.0f});
                }
            }
        } else {
            // Remove enemies that go off screen
            if (enemyPos.y > 650.0f) {
                enemyIt = enemies.erase(enemyIt);
            } else {
                ++enemyIt;
            }
        }
    }
}

void GameManager::cleanupInactiveObjects() {
    bullets.erase(
        std::remove_if(bullets.begin(), bullets.end(),
            [](const std::unique_ptr<Bullet>& bullet) {
                return !bullet->isActive();
            }),
        bullets.end()
    );
}

void GameManager::render() {
    window.clear(sf::Color::Black);
    
    player->render(window);
    
    for (auto& bullet : bullets) {
        bullet->render(window);
    }
    
    for (auto& enemy : enemies) {
        enemy->render(window);
    }
    
    renderUI();
    
    window.display();
}

void GameManager::renderUI() {
    static int frameCount = 0;
    frameCount++;
    
    if (frameCount >= 30) {
        std::cout << "\r=== INFINITE SPACE INVADERS ===";
        std::cout << " | Score: " << player->getScore();
        std::cout << " | High Score: " << highScore;
        std::cout << " | Level: " << currentLevel;
        std::cout << " | Lives: " << player->getLives();
        std::cout << " | Health: " << player->getHealth() << "%";
        
        // Health bar visualization
        std::cout << " [";
        int bars = player->getHealth() / 10;
        for (int i = 0; i < 10; i++) {
            if (i < bars) {
                if (player->getHealth() > 70) std::cout << "█";
                else if (player->getHealth() > 30) std::cout << "▒";
                else std::cout << "░";
            } else {
                std::cout << " ";
            }
        }
        std::cout << "]";
        
        // Show enemy stats
        std::cout << " | Enemies: " << enemies.size();
        
        std::cout << "   " << std::flush;
        frameCount = 0;
    }
}

void GameManager::displayMenu() {
    std::cout << "\n=== PAUSE MENU ===" << std::endl;
    std::cout << "S - Save Game" << std::endl;
    std::cout << "L - Load Game" << std::endl;
    std::cout << "R - Reset Game" << std::endl;
    std::cout << "Q - Quit Game" << std::endl;
    std::cout << "ESC - Resume Game" << std::endl;
    std::cout << "==================" << std::endl;
}

void GameManager::saveGame() {
    try {
        fileHandler.saveGame(*player, enemies, bullets);
        std::cout << "Game saved successfully!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error saving game: " << e.what() << std::endl;
    }
}

void GameManager::loadGame() {
    try {
        if (fileHandler.loadGame(*player, enemies, bullets)) {
            std::cout << "Game loaded successfully!" << std::endl;
        } else {
            std::cout << "No saved game found!" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error loading game: " << e.what() << std::endl;
    }
}

void GameManager::resetGame() {
    try {
        bullets.clear();
        enemies.clear();
        currentLevel = 1;
        player->setHealth(100);
        player->setPosition(sf::Vector2f{400.0f, 550.0f});
        initializeGame();
        std::cout << "Game reset! Starting Level 1" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error resetting game: " << e.what() << std::endl;
    }
}

void GameManager::saveHighScore() {
    if (player->getScore() > highScore) {
        highScore = player->getScore();
        std::ofstream file("highscore.txt");
        if (file.is_open()) {
            file << highScore;
            file.close();
            std::cout << "🎉 New high score saved: " << highScore << " 🎉" << std::endl;
        }
    }
}

void GameManager::loadHighScore() {
    std::ifstream file("highscore.txt");
    if (file.is_open()) {
        file >> highScore;
        file.close();
        std::cout << "High score loaded: " << highScore << std::endl;
    } else {
        highScore = 0;
    }
}

bool GameManager::validateGameState() const {
    return player != nullptr && !window.isOpen();
}

void GameManager::handleRuntimeErrors() {
    std::cout << "Recovering from error..." << std::endl;
    resetGame();
}

void GameManager::displayGameOver() {
    std::cout << "\n\n=== GAME OVER ===" << std::endl;
    std::cout << "Final Score: " << player->getScore() << std::endl;
    std::cout << "High Score: " << highScore << std::endl;
    std::cout << "Level Reached: " << currentLevel << std::endl;
    std::cout << "=================" << std::endl;
}