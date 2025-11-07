#ifndef OOP_SRENEMY_H
#define OOP_SRENEMY_H

#include "Enemy.h++"

class ShortRangeEnemy : public Enemy {
public:
    explicit ShortRangeEnemy(double x = 2.5, double y = 2.5, float heightScale = 1.0f)
        : Enemy(x, y, heightScale) {}

    [[nodiscard]] double attackRange() const override { return 1.2; }
    [[nodiscard]] int attackDamage() const override { return 90; }
    [[nodiscard]] const char* typeName() const override { return "ShortRangeEnemy"; }

    void attackPlayer(Player& player, double deltaTime, const Map &map) override ;
};

#endif // OOP_SRENEMY_H
