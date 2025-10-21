#include "FileHandler.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

FileHandler::FileHandler(const std::string& file) : filename(file) {}

void FileHandler::saveGame(const Player& player, 
                          const std::vector<std::unique_ptr<Enemy>>& enemies,
                          const std::vector<std::unique_ptr<Bullet>>& bullets) {
    
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file for writing: " + filename);
    }
    
    try {
        // Save player data
        file << "PLAYER " << player.getPosition().x << " " << player.getPosition().y << " "
             << player.getHealth() << " " << player.getScore() << " " << player.getLives() << "\n";
        
        // Save enemies
        for (const auto& enemy : enemies) {
            if (enemy->isActive() && enemy->isAlive()) {
                auto pos = enemy->getPosition();
                file << "ENEMY " << pos.x << " " << pos.y << " " 
                     << enemy->getHealth() << " " 
                     << dynamic_cast<Enemy*>(enemy.get())->getPointValue() << "\n";
            }
        }
        
        // Save bullets
        for (const auto& bullet : bullets) {
            if (bullet->isActive()) {
                auto pos = bullet->getPosition();
                file << "BULLET " << pos.x << " " << pos.y << " "
                     << (bullet->isFromPlayer() ? "1" : "0") << "\n";
            }
        }
        
        file.close();
        std::cout << "Game state saved to " << filename << std::endl;
        
    } catch (const std::exception& e) {
        file.close();
        throw std::runtime_error("Error writing to file: " + std::string(e.what()));
    }
}

bool FileHandler::loadGame(Player& player,
                          std::vector<std::unique_ptr<Enemy>>& enemies,
                          std::vector<std::unique_ptr<Bullet>>& bullets) {
    
    if (!fileExists()) {
        return false;
    }
    
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file for reading: " + filename);
    }
    
    try {
        // Clear existing objects
        enemies.clear();
        bullets.clear();
        
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string type;
            iss >> type;
            
            if (type == "PLAYER") {
                float x, y;
                int health, score, lives;
                if (iss >> x >> y >> health >> score >> lives) {
                    player.setPosition({x, y});
                    player.setHealth(health);
                }
            }
            else if (type == "ENEMY") {
                float x, y;
                int health, points;
                if (iss >> x >> y >> health >> points) {
                    auto enemy = std::make_unique<Enemy>(x, y, points);
                    enemy->setHealth(health);
                    enemies.push_back(std::move(enemy));
                }
            }
            else if (type == "BULLET") {
                float x, y;
                int isPlayer;
                if (iss >> x >> y >> isPlayer) {
                    auto bullet = std::make_unique<Bullet>(x, y, isPlayer == 1);
                    bullets.push_back(std::move(bullet));
                }
            }
        }
        
        file.close();
        std::cout << "Game state loaded from " << filename << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        file.close();
        throw std::runtime_error("Error reading from file: " + std::string(e.what()));
    }
}

bool FileHandler::fileExists() const {
    std::ifstream file(filename);
    return file.good();
}

void FileHandler::deleteSaveFile() {
    if (fileExists()) {
        std::remove(filename.c_str());
    }
}

// Helper methods for more complex parsing (stubs for now)
Player FileHandler::parsePlayerData(const std::string& line) {
    (void)line;
    // Implementation would parse complex player state
    return Player(400, 550, sf::Vector2f{800, 600});
}

std::unique_ptr<Enemy> FileHandler::parseEnemyData(const std::string& line) {
    (void)line;
    // Implementation would parse complex enemy state
    return std::make_unique<Enemy>(100, 100, 100);
}

std::unique_ptr<Bullet> FileHandler::parseBulletData(const std::string& line) {
    (void)line;
    // Implementation would parse complex bullet state
    return std::make_unique<Bullet>(100, 100, true);
}