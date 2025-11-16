#include "HDEnemy.h++"
#include <cmath>
#include "Map.h++"
#include "Player.h++"
#include <SFML/Audio.hpp>

void LongRangeHighDamageEnemy::attackPlayer(Player& player, double deltaTime, const Map &map) {
    attackCooldown_ -= static_cast<float>(deltaTime);
    if (attackCooldown_ > 0.f) return;


    double dist = Dist(player);
    if (dist <= attackRange() * 0.2) {
        SRAttackPlayer(player, deltaTime);
        return;
    }
    if (dist <= attackRange() * attackRange()  && this->hasLineOfSight(map, this->getWorldX(), this->getWorldY(), player.getX(),
                                                      player.getY())) {
        player.takeDamage(attackDamage());
        attackCooldown_ = attackInterval_;
    }
}

void LongRangeHighDamageEnemy::SRAttackPlayer(Player &player, double deltaTime) {
    attackCooldown_ -= static_cast<float>(deltaTime);
    if (attackCooldown_ > 0.f ) return;
    double dist2 = Dist(player);
    double rng = attackRange() / 6;
    if (dist2 <= rng * rng) {
        player.takeDamage(attackDamage() * 4);
        attackCooldown_ = attackInterval_ * 0.8f;
    }
}



