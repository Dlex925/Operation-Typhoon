#include "Weapon.h++"
#include <iostream>

Weapon::Weapon()
    : texture1("assets/Weapon1.png"),texture2("assets/Weapon2.png"),sprite(texture1)
{

        sf::Vector2u texSize = texture1.getSize();
        float scale = 0.75f;
        sprite.setPosition({800, 450});
        sprite.setScale({scale, scale});
        originalY = 450;


}

void Weapon::update(float deltaTime) {
    if (shooting) {
        shootTimer += deltaTime;
        if (shootTimer >= shootDuration) {
            shooting = false;
            shootTimer = 0.0f;
            sprite.setTexture(texture1);
        }
    }
}

void Weapon::shoot() {
    if (!shooting) {
        shooting = true;
        shootTimer = 0.0f;
        std::cout << "WEAPON SHOOT" << std::endl; //for debugging until enemies are implemented
        sprite.setTexture(texture2);
    }
}

void Weapon::draw(sf::RenderWindow& window) {
        window.draw(sprite);
}

 std::ostream& operator<<(std::ostream& os, const Weapon& weapon) {
     os << "\nWeapon\n" <<weapon.shootDuration << " - shootDuration\n" << weapon.shootTimer << " - shootTimer\n" << weapon.shooting << " is Shooting\n";
    return os ;
 }
