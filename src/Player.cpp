#include "Player.h++"
#include <cmath>
#include <iostream>

Player::Player(double x, double y)
    : posX(x), posY(y), dirX(-1.0f), dirY(0.0f),
      planeX(0.0f), planeY(0.66f), moveSpeed(3.5f),
      rotSpeed(1.0f), mouseSensitivity(0.0005f) {}

void Player::MoveForward(double deltaTime, const Map& map) {

    double newX = posX + dirX * moveSpeed * deltaTime;
    double newY = posY + dirY * moveSpeed * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) {
         newX = posX + dirX * moveSpeed * deltaTime * 2 ;
         newY = posY + dirY * moveSpeed * deltaTime * 2 ;
    }
    if (!map.isWall(newX, posY)) posX = newX;
    if (!map.isWall(posX, newY)) posY = newY;
}

void Player::MoveBackward(double deltaTime, const Map& map) {
    double newX = posX - dirX * moveSpeed * deltaTime;
    double newY = posY - dirY * moveSpeed * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) {
        newX = posX - dirX * moveSpeed * deltaTime * 2 ;
        newY = posY - dirY * moveSpeed * deltaTime * 2 ;
    }

    if (!map.isWall(newX, posY)) posX = newX;
    if (!map.isWall(posX, newY)) posY = newY;
}

void Player::MoveLeft(double deltaTime, const Map& map) {
    double perpDirX = -dirY;
    double perpDirY = dirX;

    double newX = posX + perpDirX * moveSpeed * deltaTime;
    double newY = posY + perpDirY * moveSpeed * deltaTime;

    if (!map.isWall(newX, posY)) posX = newX;
    if (!map.isWall(posX, newY)) posY = newY;
}

void Player::MoveRight(double deltaTime, const  Map& map) {
    double perpDirX = dirY;
    double perpDirY = -dirX;

    double newX = posX + perpDirX * moveSpeed * deltaTime;
    double newY = posY + perpDirY * moveSpeed * deltaTime;

    if (!map.isWall(newX, posY)) posX = newX;
    if (!map.isWall(posX, newY)) posY = newY;
}

void Player::RotateCamera(double mouseDeltaX) {
    double rotationAmount = mouseDeltaX * mouseSensitivity * rotSpeed;

    double oldDirX = dirX;
    dirX = dirX * cos(rotationAmount) - dirY * sin(rotationAmount);
    dirY = oldDirX * sin(rotationAmount) + dirY * cos(rotationAmount);

    double oldPlaneX = planeX;
    planeX = planeX * cos(rotationAmount) - planeY * sin(rotationAmount);
    planeY = oldPlaneX * sin(rotationAmount) + planeY * cos(rotationAmount);
}


 std::ostream& operator<<(std::ostream& os, const Player& player) {
    os << "Player(pos: (" << player.posX << ", " << player.posY
       << "), dir: (" << player.dirX << ", " << player.dirY
       << "), plane: (" << player.planeX << ", " << player.planeY << "))";
    return os;
}