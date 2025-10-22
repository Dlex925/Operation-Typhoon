#ifndef OOP_WEAPON_H
#define OOP_WEAPON_H

#include <SFML/Graphics.hpp>

class Weapon {
private:
    sf::Texture texture1, texture2;
    sf::Sprite sprite;
    bool shooting = false;
    float shootTimer = 0.0f;
    float shootDuration = 0.1f;
    float originalY;

public:
    Weapon();
    void update(float deltaTime);
    void shoot();
    void draw(sf::RenderWindow& window);
    bool isShooting() const { return shooting; }
};

#endif //OOP_WEAPON_H