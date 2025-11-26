#ifndef OOP_SRENEMY_H
#define OOP_SRENEMY_H

#include "Enemy.h++"
#include <memory>
#include <ostream>

class ShortRangeEnemy : public Enemy {
    int AttackDmg{30};

public:
    explicit ShortRangeEnemy(double x = 2.5, double y = 2.5, float heightScale = 1.0f)
        : Enemy(x, y, heightScale) {}

    [[nodiscard]] double attackRange() const override { return 1.2; }

    [[nodiscard]] int attackDamage() const override {
        return (static_cast<float>(enemy_dead) / static_cast<float>(enemy_no)) >= 0.5 ? AttackDmg * 2 : AttackDmg;
    }

    [[nodiscard]] const std::string typeName() const override { return "EnemyShort"; }

    void attackPlayer(Player& player, double deltaTime, const Map &map) override ;

    // cloning and virtual print
    [[nodiscard]] std::unique_ptr<Enemy> clone() const override { return std::make_unique<ShortRangeEnemy>(*this); }

    void print(std::ostream &os) const override {
        os << typeName() << "(hp=" << getHp() << "/" << getMaxHp() << ", x=" << getWorldX() << ", y=" << getWorldY() <<
                ")";
    }


    ShortRangeEnemy(const ShortRangeEnemy &other)
        : Enemy(other),
          AttackDmg(other.AttackDmg) {
    }

    ShortRangeEnemy(ShortRangeEnemy &&other) noexcept
        : Enemy(std::move(other)),
          AttackDmg(other.AttackDmg) {
    }

    ShortRangeEnemy &operator=(const ShortRangeEnemy &other) {
        if (this == &other)
            return *this;
        Enemy::operator =(other);
        AttackDmg = other.AttackDmg;
        return *this;
    }

    ShortRangeEnemy &operator=(ShortRangeEnemy &&other) noexcept {
        if (this == &other)
            return *this;
        Enemy::operator =(std::move(other));
        AttackDmg = other.AttackDmg;
        return *this;
    }

    friend void swap(ShortRangeEnemy &lhs, ShortRangeEnemy &rhs) noexcept {
        using std::swap;
        swap(static_cast<Enemy &>(lhs), static_cast<Enemy &>(rhs));
        swap(lhs.AttackDmg, rhs.AttackDmg);
    }
};

#endif // OOP_SRENEMY_H
