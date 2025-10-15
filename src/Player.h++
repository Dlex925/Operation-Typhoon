//
// Created by dlex on 11.10.2025.
//

#ifndef OOP_PLAYER_H
#define OOP_PLAYER_H
#include "Map.h++"

class Player {
    float posX, posY, dirX, dirY , planeX , planeY;
    const float moveSpeed , rotSpeed;
    float mouseSensitivity;
public:
    Player(float x = 1.f, float y = 1.f);
    void MoveForward(float deltaTime, Map& map);
    void MoveBackward(float deltaTime, Map& map);
    void MoveLeft(float deltaTime, Map& map);
    void MoveRight(float deltaTime, Map& map);
    void RotateCamera(float mouseDeltaX);
    // Getters
    float getX() const { return posX; }
    float getY() const { return posY; }
    float getDirX() const { return dirX; }
    float getDirY() const { return dirY; }
    float getPlaneX() const { return planeX; }
    float getPlaneY() const { return planeY; }
    void printPosition() const;

};


#endif //OOP_PLAYER_H