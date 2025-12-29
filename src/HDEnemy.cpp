#include "HDEnemy.h++"
#include <cmath>
#include "Map.h++"
#include "Player.h++"
#include <SFML/Audio.hpp>

void LongRangeHighDamageEnemy::attackPlayer(Player& player, double deltaTime, const Map &map) {



    double dist = Dist(player);
    if (dist <= attackRange() * 0.2) {
        SRAttackPlayer(player, deltaTime);
        return;
    }
    if (canAttack(player, map,deltaTime)){
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



