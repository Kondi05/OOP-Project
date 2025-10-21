#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <string>
#include <vector>
#include <memory>
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"

/*
    Handles file operations for saving and loading game state
    file I/O operations and error handling
*/
class FileHandler {
private:
    std::string filename;

public:
    FileHandler(const std::string& file = "game_state.txt");
    
    // File operations with exception handling
    void saveGame(const Player& player, 
                  const std::vector<std::unique_ptr<Enemy>>& enemies,
                  const std::vector<std::unique_ptr<Bullet>>& bullets);
    
    bool loadGame(Player& player,
                  std::vector<std::unique_ptr<Enemy>>& enemies,
                  std::vector<std::unique_ptr<Bullet>>& bullets);
    
    // Utility methods
    bool fileExists() const;
    void deleteSaveFile();

private:
    // Helper methods for parsing
    Player parsePlayerData(const std::string& line);
    std::unique_ptr<Enemy> parseEnemyData(const std::string& line);
    std::unique_ptr<Bullet> parseBulletData(const std::string& line);
};

#endif