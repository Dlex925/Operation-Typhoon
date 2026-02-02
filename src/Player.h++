#ifndef OOP_PLAYER_H
#define OOP_PLAYER_H
#include "Map.h++"
#include "Weapon.h++"
#include <algorithm>

class Player {
    double posX, posY, dirX, dirY, planeX, planeY;
    const double moveSpeed, rotSpeed;
    double mouseSensitivity;
    Weapon weapon;
    int hp_ = 1000;
    int maxHp_ = 1000;
    int ammo_ = 60;
    int score = 0;

public:
    explicit Player(double x = 1.f, double y = 1.f);

    void MoveForward(double deltaTime, const Map &map);

    void MoveBackward(double deltaTime, const Map &map);

    void MoveLeft(double deltaTime, const Map &map);

    void MoveRight(double deltaTime, const Map &map);

    void RotateCamera(double mouseDeltaX);

    friend std::ostream &operator<<(std::ostream &os, const Player &player);

    void updateWeapon(float deltaTime) { weapon.update(deltaTime); }
    void drawWeapon(sf::RenderWindow &window) { weapon.draw(window); }
    Weapon &getWeapon() { return weapon; }
    [[nodiscard]] double getX() const { return posX; }
    [[nodiscard]] double getY() const { return posY; }
    [[nodiscard]] double getDirX() const { return dirX; }
    [[nodiscard]] double getDirY() const { return dirY; }
    [[nodiscard]] double getPlaneX() const { return planeX; }
    [[nodiscard]] double getPlaneY() const { return planeY; }

    void addPoints(int a) {
        score += a;
        score = std::max(score, 0);
    }

    int score_() {
        return score;
    }

    void setPosition(double x, double y) {
        posX = x;
        posY = y;
    }

    void resetForNewGame() {
        hp_ = maxHp_;
        ammo_ = 30;
    }

    void enableGodMode() {
        maxHp_ = 10000000;
        hp_ = maxHp_;
    }

    [[nodiscard]] int getHp() const { return hp_; }
    [[nodiscard]] int getMaxHp() const { return maxHp_; }
    [[nodiscard]] int getAmmo() const { return ammo_; }
    void addAmmo(int a) { ammo_ = std::min(ammo_ + std::max(0, a), 999); }
    void heal(int h) { hp_ = std::min(hp_ + std::max(0, h), maxHp_); }
    void takeDamage(int d) { hp_ = std::max(0, hp_ - std::max(0, d)); }

    bool consumeAmmo(int a) {
        if (a <= 0) return true;
        if (ammo_ < a) return false;
        ammo_ -= a;
        return true;
    }
};


#endif //OOP_PLAYER_H
