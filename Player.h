#ifndef _PLAYER_H_
#define _PLAYER_H_
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;

class Player {
private:
    RectangleShape* body; 
    Player* player;
    int depth; //for hitbox
    int width; //for hitbox
public:
    //to add functionality into this!!

};

#endif