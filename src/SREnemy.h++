#ifndef OOP_SRENEMY_H
#define OOP_SRENEMY_H

#include "Enemy.h++"
#include <memory>
#include <ostream>

class ShortRangeEnemy : public Enemy {
public:
    explicit ShortRangeEnemy(double x = 2.5, double y = 2.5, float heightScale = 1.0f)
        : Enemy(x, y, heightScale) {}

    [[nodiscard]] double attackRange() const override { return 1.2; }
    [[nodiscard]] int attackDamage() const override { return 30; }
    [[nodiscard]] const char* typeName() const override { return "ShortRangeEnemy"; }

    void attackPlayer(Player& player, double deltaTime, const Map &map) override ;

    // cloning and virtual print
    [[nodiscard]] std::unique_ptr<Enemy> clone() const override { return std::make_unique<ShortRangeEnemy>(*this); }
    void print(std::ostream &os) const override {
        os << typeName() << "(hp=" << getHp() << "/" << getMaxHp() << ", x=" << getWorldX() << ", y=" << getWorldY() << ")";
    }
};

#endif // OOP_SRENEMY_H
