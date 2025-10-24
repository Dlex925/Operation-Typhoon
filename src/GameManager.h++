#ifndef OOP_GAMEMANAGER_H
#define OOP_GAMEMANAGER_H

#include "Map.h++"
#include "Player.h++"
#include "Raycast.h++"
#include "Enemy.h++"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class GameManager {

    sf::RenderWindow& window;
    Map map;
    Player& player;
    Raycast raycast;
    std::vector<Enemy> enemies;


    void performHitscanDamage();

    void performShortRangeAttackDamage();

public:
    GameManager(sf::RenderWindow& win, std::string harta, Player& p);

    void start();

    void handleInput(float deltaTime);

    void Engine() const;

friend std::ostream& operator<<(std::ostream& os, const GameManager& gm) {


    os << "MAP:\n";
    os << gm.map;

    os << "\nPLAYER:\n";
    os << gm.player;

    os << "\nRAYCAST:\n";
    os << gm.raycast;

    os << "\nWEAPON:\n";
    os << const_cast<GameManager &>(gm).player.getWeapon();

    return os;
}
};

#endif //OOP_GAMEMANAGER_H