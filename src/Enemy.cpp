#include "Enemy.h++"
#include "Map.h++"
#include "Player.h++"
#include <cmath>

Enemy::Enemy(double x, double y, float heightScale, int hp1)
    : worldX(x), worldY(y), baseHeight(heightScale), maxHp(hp1), hp(hp1) {
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

// DDA line-of-sight helper
bool Enemy::hasLineOfSight(const Map &map, double sx, double sy, double tx, double ty) {
    int mapX = static_cast<int>(sx);
    int mapY = static_cast<int>(sy);
    int targetX = static_cast<int>(tx);
    int targetY = static_cast<int>(ty);

    double dirX = tx - sx;
    double dirY = ty - sy;
    double len = std::sqrt(dirX * dirX + dirY * dirY);
    if (len < 1e-6) return true;
    dirX /= len;
    dirY /= len;

    double deltaDistX = (dirX == 0.0) ? 1e30 : std::abs(1.0 / dirX);
    double deltaDistY = (dirY == 0.0) ? 1e30 : std::abs(1.0 / dirY);

    int stepX = (dirX < 0) ? -1 : 1;
    int stepY = (dirY < 0) ? -1 : 1;

    double sideDistX = (dirX < 0)
                           ? (sx - mapX) * deltaDistX
                           : (static_cast<double>(mapX) + 1.0 - sx) * deltaDistX;
    double sideDistY = (dirY < 0)
                           ? (sy - mapY) * deltaDistY
                           : (static_cast<double>(mapY) + 1.0 - sy) * deltaDistY;

    int maxSteps = 4096;
    while (maxSteps-- > 0) {
        if (mapX == targetX && mapY == targetY) return true;
        if (sideDistX < sideDistY) {
            sideDistX += deltaDistX;
            mapX += stepX;
        } else {
            sideDistY += deltaDistY;
            mapY += stepY;
        }
        if (map.isWall(static_cast<double>(mapX), static_cast<double>(mapY))) return false;
    }
    return false;
}

void Enemy::update(double deltaTime, const Map &map, const Player &player) {
    double dx = player.getX() - getWorldX();
    double dy = player.getY() - getWorldY();
    double dist = std::sqrt(dx*dx + dy*dy);

    if (dist > attackRange() * 0.9) {
        double step = 0.1;
        step = std::min(1.5 * deltaTime, dist);
        double nx = getWorldX() + (dx / (dist + 1e-6)) * step;
        double ny = getWorldY() + (dy / (dist + 1e-6)) * step;
        if (!map.isWall(nx, ny)) {
            setWorldPosition(nx, ny);
        }
    }

    attackPlayer(const_cast<Player&>(player), deltaTime,map);

}

