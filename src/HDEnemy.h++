#ifndef OOP_HDENEMY_H
#define OOP_HDENEMY_H

#include "Enemy.h++"
#include <SFML/Audio.hpp>
#include <memory>
#include <ostream>

class LongRangeHighDamageEnemy : public Enemy {
int AttackDmg{50};

public:
void DoubleDamage() { AttackDmg *= 2; }

explicit LongRangeHighDamageEnemy(double x = 2.5, double y = 2.5, float heightScale = 1.0f)
    : Enemy(x, y, heightScale, 150) {
}

[[nodiscard]] double attackRange() const override { return 6.0; }

[[nodiscard]] int attackDamage() const override {
    return (static_cast<float>(enemy_dead) / static_cast<float>(enemy_no)) >= 0.5 ? AttackDmg * 2 : AttackDmg;
}

[[nodiscard]] const std::string typeName() const override { return "EnemyLongHD"; }

void attackPlayer(Player &player, double deltaTime, const Map &map) override;

//void update(double deltaTime, const Map &map, const Player &player) override;
private: //Short range attack
void SRAttackPlayer(Player &player, double deltaTime);
public :
[[nodiscard]] std::unique_ptr<Enemy> clone() const override {
    return std::make_unique<LongRangeHighDamageEnemy>(*this);
}

void print(std::ostream &os) const override {
    os << typeName() << "(hp=" << getHp() << "/" << getMaxHp() << ", x=" << getWorldX() << ", y=" << getWorldY() <<
            ")" << "\nDead_Enemies " << enemy_dead << "\nTotal Enemies " << enemy_no << '\n' << (
                static_cast<float>(enemy_dead) / static_cast<float>(enemy_no) >= 0.5) << '\n' << static_cast<float>(
                    enemy_dead) / static_cast<float>(enemy_no) << "\n";
    }

LongRangeHighDamageEnemy(const LongRangeHighDamageEnemy &other)
    : Enemy(other),
      AttackDmg(other.AttackDmg) {
}

LongRangeHighDamageEnemy(LongRangeHighDamageEnemy &&other) noexcept
    : Enemy(std::move(other)),
      AttackDmg(other.AttackDmg) {
}

LongRangeHighDamageEnemy &operator=(const LongRangeHighDamageEnemy &other) {
    if (this == &other)
        return *this;
    Enemy::operator =(other);
    AttackDmg = other.AttackDmg;
    return *this;
}

LongRangeHighDamageEnemy &operator=(LongRangeHighDamageEnemy &&other) noexcept {
    if (this == &other)
        return *this;
    Enemy::operator =(std::move(other));
    AttackDmg = other.AttackDmg;
    return *this;
}

friend void swap(LongRangeHighDamageEnemy &lhs, LongRangeHighDamageEnemy &rhs) noexcept {
    using std::swap;
    swap(static_cast<Enemy &>(lhs), static_cast<Enemy &>(rhs));
    swap(lhs.AttackDmg, rhs.AttackDmg);
}
};

#endif // OOP_HDENEMY_H
