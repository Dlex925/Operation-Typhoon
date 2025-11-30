#ifndef OOP_GAMEMANAGER_H
#define OOP_GAMEMANAGER_H

#include "Map.h++"
#include "Player.h++"
#include "Raycast.h++"
#include "Enemy.h++"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <vector>
#include <memory>

#include "Pickup.h++"

class GameManager {
    sf::RenderWindow& window;
    Map map;
    Player& player;
    Raycast raycast;
    std::vector<std::unique_ptr<Enemy> > enemies;
    std::vector<std::unique_ptr<PickupBase> > pickups;

    bool paused_ = false;
    bool pHeld_ = false;
    bool rHeld_ = false;
    sf::Font uiFont;
    bool uiFontLoaded_ = false;
    sf::Music music;

    bool gameOver_ = false;
    float gameOverTimer_ = 0.f;
    bool gameOverSoundPlayed_ = false;
    sf::Music gameOverJingle_;
    sf::Music winmusic;
    //For restarting
    double initialPlayerX_ = 0.0;
    double initialPlayerY_ = 0.0;
    int curentMapIndex_ = 1;
    bool levelComplete_ = false;
    bool gameFin = false;

    void loadNextLevel();

    void spawnEnemiesFromMap();
    void restartGame_();
public:
    GameManager(sf::RenderWindow& win, std::string harta, Player& p);

    void start();

    void handleInput(float deltaTime);

    void spawnPickupsFromMap();

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