#ifndef OOP_HDENEMY_H
#define OOP_HDENEMY_H

#include "Enemy.h++"
#include <SFML/Audio.hpp>
class LongRangeHighDamageEnemy : public Enemy {

public:
    explicit LongRangeHighDamageEnemy(double x = 2.5, double y = 2.5, float heightScale = 1.0f)
        : Enemy(x, y, heightScale) {}

    [[nodiscard]] double attackRange() const override { return 6.0; }
    [[nodiscard]] int attackDamage() const override { return 99; }
    [[nodiscard]] const char* typeName() const override { return "LongRangeHighDamageEnemy"; }

    void attackPlayer(Player& player, double deltaTime, const Map &map) override;
    void update(double deltaTime, const Map &map, const Player &player) override;
    void SRAttackPlayer(Player &player, double deltaTime) ;

};

#endif // OOP_HDENEMY_H
