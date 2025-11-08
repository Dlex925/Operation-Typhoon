#ifndef OOP_LRENEMY_H
#define OOP_LRENEMY_H

#include "Enemy.h++"
#include <memory>
#include <ostream>

class LongRangeEnemy : public Enemy {
public:
    explicit LongRangeEnemy(double x = 2.5, double y = 2.5, float heightScale = 1.0f)
        : Enemy(x, y, heightScale, 500) {
    }

    [[nodiscard]] double attackRange() const override { return 10.0; }
    [[nodiscard]] int attackDamage() const override { return (static_cast<float>(enemy_dead)/static_cast<float>(enemy_no)) >= 0.5 ? 30 : 15; }
    [[nodiscard]] const char* typeName() const override { return "LongRangeEnemy"; }

    void attackPlayer(Player& player, double deltaTime, const Map &map) override;

    //void update(double deltaTime, const Map &map, const Player &player) override;

    [[nodiscard]] std::unique_ptr<Enemy> clone() const override { return std::make_unique<LongRangeEnemy>(*this); }

    void print(std::ostream &os) const override {
        os << typeName() << "(hp=" << getHp() << "/" << getMaxHp() << ", x=" << getWorldX() << ", y=" << getWorldY() <<
                ")";
    }
};

#endif // OOP_LRENEMY_H
