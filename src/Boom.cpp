//
// Created by dlex on 26.11.2025.
//

#include "Boom.h++"


void Boom::attackPlayer(Player &player, double deltaTime, const Map &map) {
    if (isDead()) return;
    attackCooldown_ -= static_cast<float>(deltaTime);
    if (attackCooldown_ > 0.f) return;
    double dist = Dist(player);
    if (dist <= attackRange() * attackRange() && this->hasLineOfSight(map, this->getWorldX(), this->getWorldY(),
                                                                      player.getX(), player.getY())) {
        player.takeDamage(attackDamage());
        takeDamage(500000);
    }
}
