#ifndef OOP_LRENEMY_H
#define OOP_LRENEMY_H

#include "Enemy.h++"
#include <memory>
#include <ostream>

class LongRangeEnemy : public Enemy {
    int AttackDmg{15};

public:
    explicit LongRangeEnemy(double x = 2.5, double y = 2.5, float heightScale = 1.0f)
        : Enemy(x, y, heightScale, 500) {
    }

    [[nodiscard]] double attackRange() const override { return 10.0; }

    [[nodiscard]] int attackDamage() const override {
        return (static_cast<float>(enemy_dead) / static_cast<float>(enemy_no)) >= 0.5 ? AttackDmg * 2 : AttackDmg;
    }

    [[nodiscard]] const std::string typeName() const override { return "EnemyLong"; }

    void attackPlayer(Player& player, double deltaTime, const Map &map) override;

    //void update(double deltaTime, const Map &map, const Player &player) override;

    [[nodiscard]] std::unique_ptr<Enemy> clone() const override { return std::make_unique<LongRangeEnemy>(*this); }

    void print(std::ostream &os) const override {
        os << typeName() << "(hp=" << getHp() << "/" << getMaxHp() << ", x=" << getWorldX() << ", y=" << getWorldY() <<
                ")";
    }

    LongRangeEnemy(const LongRangeEnemy &other)
        : Enemy(other),
          AttackDmg(other.AttackDmg) {
    }

    LongRangeEnemy(LongRangeEnemy &&other) noexcept
        : Enemy(std::move(other)),
          AttackDmg(other.AttackDmg) {
    }

    LongRangeEnemy &operator=(const LongRangeEnemy &other) {
        if (this == &other)
            return *this;
        Enemy::operator =(other);
        AttackDmg = other.AttackDmg;
        return *this;
    }

    LongRangeEnemy &operator=(LongRangeEnemy &&other) noexcept {
        if (this == &other)
            return *this;
        Enemy::operator =(std::move(other));
        AttackDmg = other.AttackDmg;
        return *this;
    }

    friend void swap(LongRangeEnemy &lhs, LongRangeEnemy &rhs) noexcept {
        using std::swap;
        swap(static_cast<Enemy &>(lhs), static_cast<Enemy &>(rhs));
        swap(lhs.AttackDmg, rhs.AttackDmg);
    }
};

#endif // OOP_LRENEMY_H
