#include "Weapon.h++"
#include <iostream>

Weapon::Weapon()
    : texture1("assets/Weapon1.png"),texture2("assets/Weapon2.png"),sprite(texture1)
{

    std::cout << "=== WEAPON CONSTRUCTOR ===" << std::endl;

    bool loaded1 = texture1.loadFromFile("assets/Weapon1.png");
    bool loaded2 = texture2.loadFromFile("assets/Weapon2.png");

    std::cout << "Textures loaded: " << loaded1 << ", " << loaded2 << std::endl;

    if (loaded1) {
        sf::Vector2u texSize = texture1.getSize();
        std::cout << "Texture size: " << texSize.x << "x" << texSize.y << std::endl;

        float scale = 2.0f;

        sprite.setPosition({800, 450});
        sprite.setScale({scale, scale});
        originalY = 450;

    }
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
        std::cout << "WEAPON: SHOOT!" << std::endl;
        sprite.setTexture(texture2);
    }
}

void Weapon::draw(sf::RenderWindow& window) {
        window.draw(sprite);
}