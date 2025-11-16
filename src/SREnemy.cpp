#include "SREnemy.h++"
#include "Player.h++"
#include <SFML/Audio.hpp>

void ShortRangeEnemy::attackPlayer(Player &player, double deltaTime,const Map &map) {
    attackCooldown_ -= static_cast<float>(deltaTime);
    if (attackCooldown_ > 0.f) return;
    double dist = Dist(player);
    if (dist <= attackRange() * attackRange() && hasLineOfSight(map, this->getWorldX(), this->getWorldY(),
                                                                player.getX(), player.getY())) {
        player.takeDamage(attackDamage());
        attackCooldown_ = attackInterval_ * 0.8f;
    }
}
