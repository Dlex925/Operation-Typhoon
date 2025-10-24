#ifndef OOP_WEAPON_H
#define OOP_WEAPON_H

#include <SFML/Graphics.hpp>
#include <vector>

class Map;
class Player;
class Enemy;

class Weapon {
private:
    sf::Texture texture1, texture2;
    sf::Texture knifeTexture;
    sf::Sprite sprite;
    bool shooting = false;
    float shootTimer = 0.0f;
    float shootDuration = 0.35f;
    float originalY{};

    static int hitscanClosestEnemy(const Map &map, const Player &player,
                                   const std::vector<Enemy> &enemies,
                                   double rangeCap, double hitWidth);

public:
    Weapon();
    void update(float deltaTime);

    bool shootPrimary(const Map &map, const Player &player, std::vector<Enemy> &enemies);

    bool shootSecondary(const Map &map, const Player &player, std::vector<Enemy> &enemies);

    void const draw(sf::RenderWindow &window) const;

    friend std::ostream& operator<<(std::ostream& os, const Weapon& weapon) ;

};

#endif //OOP_WEAPON_H