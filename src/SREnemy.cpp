#include "SREnemy.h++"
#include "Player.h++"
#include <SFML/Audio.hpp>

void ShortRangeEnemy::attackPlayer(Player &player, double deltaTime,const Map &map) {
    if (canAttack(player, map, deltaTime)) {
        player.takeDamage(attackDamage());
        attackCooldown_ = attackInterval_ * 0.8f;
    }
}
