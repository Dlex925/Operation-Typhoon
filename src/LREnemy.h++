#ifndef OOP_LRENEMY_H
#define OOP_LRENEMY_H

#include "Enemy.h++"

class LongRangeEnemy : public Enemy {
public:
    explicit LongRangeEnemy(double x = 2.5, double y = 2.5, float heightScale = 1.0f)
        : Enemy(x, y, heightScale) {}

    [[nodiscard]] double attackRange() const override { return 6.0; }
    [[nodiscard]] int attackDamage() const override { return 12; }
    [[nodiscard]] const char* typeName() const override { return "LongRangeEnemy"; }

    void attackPlayer(Player& player, double deltaTime, const Map &map) override;
    void update(double deltaTime, const Map &map, const Player &player) override;

};

#endif // OOP_LRENEMY_H
