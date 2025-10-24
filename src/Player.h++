
#ifndef OOP_PLAYER_H
#define OOP_PLAYER_H
#include "Map.h++"
#include "Weapon.h++"
class Player {
    double posX, posY, dirX, dirY , planeX , planeY;
    const double moveSpeed  , rotSpeed;
    double mouseSensitivity;
    Weapon weapon;
public:
    explicit Player(double x = 1.f, double y = 1.f);
    void MoveForward(double deltaTime, const Map& map);
    void MoveBackward(double deltaTime, const Map& map);
    void MoveLeft(double deltaTime, const Map& map);
    void MoveRight(double deltaTime, const Map& map);
    void RotateCamera(double mouseDeltaX);
    friend std::ostream& operator<<(std::ostream& os, const Player& player) ;
    void updateWeapon(float deltaTime) { weapon.update(deltaTime); }
    void drawWeapon(sf::RenderWindow& window) { weapon.draw(window); }
    Weapon& getWeapon() { return weapon; }
    // Getters
    [[nodiscard]]double getX() const { return posX; }
    [[nodiscard]]double getY() const { return posY; }
    [[nodiscard]]double getDirX() const { return dirX; }
    [[nodiscard]] double getDirY() const { return dirY; }
    [[nodiscard]] double getPlaneX() const { return planeX; }
    [[nodiscard]]double getPlaneY() const { return planeY; }


};


#endif //OOP_PLAYER_H