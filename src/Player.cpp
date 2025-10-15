#include "Player.h++"
#include <cmath>
#include <iostream>

Player::Player(float x, float y)
    : posX(x), posY(y), dirX(-1.0f), dirY(0.0f),
      planeX(0.0f), planeY(0.66f), moveSpeed(3.5f),
      rotSpeed(1.0f), mouseSensitivity(0.0005f) {}

void Player::MoveForward(float deltaTime, Map& map) {
    float newX = posX + dirX * moveSpeed * deltaTime;
    float newY = posY + dirY * moveSpeed * deltaTime;

    if (!map.isWall(newX, posY)) posX = newX;
    if (!map.isWall(posX, newY)) posY = newY;
}

void Player::MoveBackward(float deltaTime, Map& map) {
    float newX = posX - dirX * moveSpeed * deltaTime;
    float newY = posY - dirY * moveSpeed * deltaTime;

    if (!map.isWall(newX, posY)) posX = newX;
    if (!map.isWall(posX, newY)) posY = newY;
}

void Player::MoveLeft(float deltaTime, Map& map) {
    float perpDirX = -dirY;
    float perpDirY = dirX;

    float newX = posX + perpDirX * moveSpeed * deltaTime;
    float newY = posY + perpDirY * moveSpeed * deltaTime;

    if (!map.isWall(newX, posY)) posX = newX;
    if (!map.isWall(posX, newY)) posY = newY;
}

void Player::MoveRight(float deltaTime, Map& map) {
    float perpDirX = dirY;
    float perpDirY = -dirX;

    float newX = posX + perpDirX * moveSpeed * deltaTime;
    float newY = posY + perpDirY * moveSpeed * deltaTime;

    if (!map.isWall(newX, posY)) posX = newX;
    if (!map.isWall(posX, newY)) posY = newY;
}

void Player::RotateCamera(float mouseDeltaX) {
    float rotationAmount = mouseDeltaX * mouseSensitivity;

    float oldDirX = dirX;
    dirX = dirX * cos(rotationAmount) - dirY * sin(rotationAmount);
    dirY = oldDirX * sin(rotationAmount) + dirY * cos(rotationAmount);

    float oldPlaneX = planeX;
    planeX = planeX * cos(rotationAmount) - planeY * sin(rotationAmount);
    planeY = oldPlaneX * sin(rotationAmount) + planeY * cos(rotationAmount);
}

void Player::printPosition() const {
    std::cout << "Player pos: (" << posX << ", " << posY
              << ") dir: (" << dirX << ", " << dirY << ")" << std::endl;
}