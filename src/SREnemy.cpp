#include "SREnemy.h++"
#include "Player.h++"
#include <SFML/Audio.hpp>
void ShortRangeEnemy::attackPlayer(Player &player, double deltaTime,const Map &map) {
    attackCooldown_ -= static_cast<float>(deltaTime);
    if (attackCooldown_ > 0.f || isDead()) return;
    double dx = getWorldX() - player.getX();
    double dy = getWorldY() - player.getY();
    double dist2 = dx*dx + dy*dy;
    double rng = attackRange();
    if (dist2 <= rng * rng && hasLineOfSight(map, this->getWorldX(), this->getWorldY(), player.getX(), player.getY())) {
        player.takeDamage(attackDamage());
        attackCooldown_ = attackInterval_ * 0.8f;
    }
}
