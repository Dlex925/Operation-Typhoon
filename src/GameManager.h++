#ifndef OOP_GAMEMANAGER_H
#define OOP_GAMEMANAGER_H

#include "Map.h++"
#include "Player.h++"
#include "Raycast.h++"
#include <SFML/Graphics.hpp>
#include <string>

class GameManager {

    sf::RenderWindow& window;
    Map map;
    Player& player;
    Raycast raycast;


public:
    GameManager(sf::RenderWindow& win, std::string harta, Player& p);
    void start()const;
    void handleInput(float deltaTime)const;
    void Engine() const;

friend std::ostream& operator<<(std::ostream& os, const GameManager& gm) {


    os << "MAP:\n";
    os << gm.map;

    os << "\nPLAYER:\n";
    os << gm.player;

    os << "\nRAYCAST:\n";
    os << gm.raycast;

    return os;
}
};

#endif //OOP_GAMEMANAGER_H