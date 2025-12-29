//
// Created by dlex on 26.11.2025.
//

#include "Boom.h++"


void Boom::attackPlayer(Player &player, double deltaTime, const Map &map) {
    if (isDead()) return;

    if (!triggered) {
        if (canAttack(player, map, deltaTime)) {
            triggered = true;
        }
    } else {
        fuseTimer -= static_cast<float>(deltaTime);
        if (fuseTimer <= 0.0f) {
            player.takeDamage(attackDamage());

            takeDamage(999999);
        }
    }
}
