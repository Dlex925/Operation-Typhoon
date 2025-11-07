#include "LREnemy.h++"
#include "Map.h++"
#include "Player.h++"
#include <cmath>


void LongRangeEnemy::attackPlayer(Player& player, double deltaTime,const Map &map) {
    attackCooldown_ -= static_cast<float>(deltaTime);
    if (attackCooldown_ > 0.f) return;

    double dx = player.getX() - getWorldX();
    double dy = player.getY() - getWorldY();
    double dist = std::sqrt(dx*dx + dy*dy);
    if (dist <= attackRange() && this->hasLineOfSight(map,this->getWorldX(),this->getWorldY(), player.getX(), player.getY())) {
        player.takeDamage(attackDamage());
        attackCooldown_ = attackInterval_;
    }
}

void LongRangeEnemy::update(double deltaTime, const Map &map, const Player &player) {
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
