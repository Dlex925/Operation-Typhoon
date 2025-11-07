#include "HDEnemy.h++"
#include <cmath>
#include "Map.h++"
#include "Player.h++"

void LongRangeHighDamageEnemy::attackPlayer(Player& player, double deltaTime, const Map &map) {
    attackCooldown_ -= static_cast<float>(deltaTime);
    if (attackCooldown_ > 0.f) return;

    double dx = player.getX() - getWorldX();
    double dy = player.getY() - getWorldY();
    double dist = std::sqrt(dx*dx + dy*dy);
    if (dist <= attackRange() && this->hasLineOfSight(map,this->getWorldX(),this->getWorldY(), player.getX(), player.getY()) && dist >= attackRange() * 0.2) {
        player.takeDamage(attackDamage());
        attackCooldown_ = attackInterval_;
    }
}

void LongRangeHighDamageEnemy::SRAttackPlayer(Player &player, double deltaTime) {
    attackCooldown_ -= static_cast<float>(deltaTime);
    if (attackCooldown_ > 0.f ) return;
    double dx = getWorldX() - player.getX();
    double dy = getWorldY() - player.getY();
    double dist2 = dx*dx + dy*dy;
    double rng = attackRange();
    if (dist2 <= rng*rng) {
        player.takeDamage(attackDamage() * 500);
        attackCooldown_ = attackInterval_ * 0.8f;
    }
}

void LongRangeHighDamageEnemy::update(double deltaTime, const Map &map, const Player &player) {
    double dx = player.getX() - getWorldX();
    double dy = player.getY() - getWorldY();
    double dist = std::sqrt(dx*dx + dy*dy);

    double step = 0.0;
    if (dist > attackRange() * 0.1) {
        step = std::min(1.5 * deltaTime, dist);
        double nx = getWorldX() + (dx / (dist + 1e-6)) * step;
        double ny = getWorldY() + (dy / (dist + 1e-6)) * step;
        if (!map.isWall(nx, ny)) {
            setWorldPosition(nx, ny);
        }
    }

    attackPlayer(const_cast<Player&>(player), deltaTime,map);
    SRAttackPlayer(const_cast<Player&>(player), deltaTime);
}

