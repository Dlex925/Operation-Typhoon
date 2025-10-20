//
// Created by dlex on 11.10.2025.
//

#ifndef OOP_PLAYER_H
#define OOP_PLAYER_H
#include "Map.h++"

class Player {
    double posX, posY, dirX, dirY , planeX , planeY;
    const double moveSpeed  , rotSpeed;
    double mouseSensitivity;
public:
    explicit Player(double x = 1.f, double y = 1.f);
    void MoveForward(double deltaTime, const Map& map);
    void MoveBackward(double deltaTime, const Map& map);
    void MoveLeft(double deltaTime, const Map& map);
    void MoveRight(double deltaTime, const Map& map);
    void RotateCamera(double mouseDeltaX);
    friend std::ostream& operator<<(std::ostream& os, const Player& player) ;
    // Getters
    [[nodiscard]]double getX() const { return posX; }
    [[nodiscard]]double getY() const { return posY; }
    [[nodiscard]]double getDirX() const { return dirX; }
    [[nodiscard]] double getDirY() const { return dirY; }
    [[nodiscard]] double getPlaneX() const { return planeX; }
    [[nodiscard]]double getPlaneY() const { return planeY; }
    //void printPosition() const;

};


#endif //OOP_PLAYER_H