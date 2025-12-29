#include "Enemy.h++"
#include "Map.h++"
#include "Player.h++"
#include <cmath>

#include "HDEnemy.h++"

int Enemy::enemy_no = 0;
int Enemy::enemy_dead = 0;

Enemy::Enemy(double x, double y, float heightScale, int hp1, double mvspeed)
    : worldX(x), worldY(y), baseHeight(heightScale), moveSpeed(mvspeed), maxHp(hp1), hp(hp1) {
    enemy_no++;
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
    LongRangeHighDamageEnemy *sre = dynamic_cast<LongRangeHighDamageEnemy *>(this);
    if (sre != nullptr) {
        sre->DoubleDamage();
    }
    if (hp <= 0) {
        enemy_dead++;
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
    double dist = Dist(player);

    if (dist > attackRange() * 0.9) {
        double nextX, nextY;

        if (hasLineOfSight(map, getWorldX(), getWorldY(), player.getX(), player.getY())) {
            nextX = player.getX();
            nextY = player.getY();
            currentPath.clear();
        } else {
            pathTimer -= deltaTime;

            if (pathTimer <= 0.0f || currentPath.empty()) {
                Pathfinder pf(map);
                currentPath = pf.findPath(getWorldX(), getWorldY(), player.getX(), player.getY());
                pathTimer = 0.2f;
            }

            if (!currentPath.empty()) {
                nextX = currentPath[0].x + 0.5;
                nextY = currentPath[0].y + 0.5;

                double dToNode = std::sqrt(std::pow(nextX - getWorldX(), 2) + std::pow(nextY - getWorldY(), 2));
                if (dToNode < 0.1) {
                    currentPath.erase(currentPath.begin());
                    if (!currentPath.empty()) {
                        nextX = currentPath[0].x + 0.5;
                        nextY = currentPath[0].y + 0.5;
                    }
                }
            } else {
                return;
            }
        }

        double dx = nextX - getWorldX();
        double dy = nextY - getWorldY();
        double distToTarget = std::sqrt(dx * dx + dy * dy);

        double step = std::min(moveSpeed * deltaTime, distToTarget);

        double moveX = (dx / (distToTarget + 1e-6)) * step;
        double moveY = (dy / (distToTarget + 1e-6)) * step;

        double nx = getWorldX() + moveX;
        double ny = getWorldY() + moveY;

        if (!map.isWall(nx, ny)) {
            setWorldPosition(nx, ny);
        }
    }

    attackPlayer(const_cast<Player &>(player), deltaTime, map);
}

//
bool Enemy::canAttack(const Player &player, const Map &map, double deltaTime) {
    attackCooldown_ -= static_cast<float>(deltaTime);
    if (attackCooldown_ > 0.f) return false;
    double distSq = Dist(player);
    double rng = attackRange();
    if (distSq > rng * rng) return false;
    return hasLineOfSight(map, getWorldX(), getWorldY(), player.getX(), player.getY());
}
