#include "Enemy.h++"
#include "Map.h++"
#include "Player.h++"
#include <cmath>

Enemy::Enemy(double x, double y, float heightScale)
    : worldX(x), worldY(y), baseHeight(heightScale) {
}

void Enemy::setWorldPosition(double x, double y) {
    worldX = x;
    worldY = y;
}

double Enemy::getWorldX() const { return worldX; }

double Enemy::getWorldY() const { return worldY; }

float Enemy::getBaseHeight() const { return baseHeight; }

int Enemy::getHp() const { return hp; }

int Enemy::getMaxHp() const { return maxHp; }

bool Enemy::isDead() const { return dead; }

void Enemy::takeDamage(int amount) {
    if (dead) return;
    hp -= amount;
    if (hp <= 0) {
        dead = true;
    }
}


void Enemy::update(double deltaTime, const Map &map, const Player &player) {
    if (dead) return;
    double dx = player.getX() - worldX;
    double dy = player.getY() - worldY;
    double dist2 = dx * dx + dy * dy;
    double stopDist2 = stopDistance * stopDistance;
    if (dist2 <= stopDist2) return;
    double dist = std::sqrt(dist2);
    if (dist < 1e-6) return;
    double step = moveSpeed * deltaTime;
    double dirX = dx / dist;
    double dirY = dy / dist;
    double newX = worldX + dirX * step;
    double newY = worldY + dirY * step;
    if (!map.isWall(newX, worldY)) worldX = newX;
    if (!map.isWall(worldX, newY)) worldY = newY;
}
