#ifndef OOP_RAYCAST_H
#define OOP_RAYCAST_H
#include "Player.h++"
#include "SFML/Graphics/RenderWindow.hpp"


class Raycast {
    sf::RenderWindow *window;
    Map* map;
    Player* player;
public:
    Raycast(sf::RenderWindow& win, Map& m , Player& p);
    void render();
};


#endif //OOP_RAYCAST_H