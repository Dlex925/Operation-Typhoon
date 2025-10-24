#ifndef OOP_RAYCAST_H
#define OOP_RAYCAST_H
#include "Player.h++"
#include "Enemy.h++"
#include <SFML/Graphics.hpp>
#include <vector>


class Raycast {
    sf::RenderWindow *window;
    Map* map;
    Player* player;
    mutable std::vector<double> depthBuffer;
public:
    Raycast(sf::RenderWindow& win, Map& m , Player& p);
    void render() const;
    void renderEnemy(const Enemy& enemy) const;

friend std::ostream& operator<<(std::ostream& os, const Raycast& raycast) ;
};
#endif //OOP_RAYCAST_H