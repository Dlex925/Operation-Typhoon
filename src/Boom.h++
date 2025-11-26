//
// Created by dlex on 26.11.2025.
//

#ifndef OOP_BOOM_H
#define OOP_BOOM_H
#include "Enemy.h++"


class Boom : public Enemy {
    int AttackDmg{500};

public:
    explicit Boom(double x = 2.5, double y = 2.5, float heightScale = 1.0f, double mvspeed = 5)
        : Enemy(x, y, heightScale, 200, mvspeed) {
    }

    [[nodiscard]] double attackRange() const override { return 0.8; }

    [[nodiscard]] int attackDamage() const override { return AttackDmg; }
    [[nodiscard]] const std::string typeName() const override { return "Boom"; }

    void attackPlayer(Player &player, double deltaTime, const Map &map) override;

    [[nodiscard]] std::unique_ptr<Enemy> clone() const override { return std::make_unique<Boom>(*this); }

    void print(std::ostream &os) const override {
        os << typeName() << "(hp=" << getHp() << "/" << getMaxHp() << ", x=" << getWorldX() << ", y=" << getWorldY() <<
                ")";
    }

    Boom(const Boom &other) = default;

    Boom(Boom &&other) noexcept = default;

    Boom &operator=(const Boom &other) = default;

    Boom &operator=(Boom &&other) noexcept = default;

    friend void swap(Boom &lhs, Boom &rhs) noexcept {
        using std::swap;
        swap(static_cast<Enemy &>(lhs), static_cast<Enemy &>(rhs));
        swap(lhs.AttackDmg, rhs.AttackDmg);
    }
};


#endif //OOP_BOOM_H
