#ifndef OOP_ENEMY_H
#define OOP_ENEMY_H

#include <cstddef>

class Map;
class Player;

class Enemy {
    double worldX{2.5};
    double worldY{2.5};
    float baseHeight{1.2f};
    double moveSpeed{2.0};
    double stopDistance{0.75};
    int maxHp{100};
    int hp{100};
    bool dead{false};
    int typeCode_{1};

protected:
    float attackInterval_{0.5f};
    float attackCooldown_{0.f};

    static bool hasLineOfSight(const Map &map, double sx, double sy, double tx, double ty);

public:
    explicit Enemy(double x = 2.5, double y = 2.5, float heightScale = 0.9f);

    virtual ~Enemy() = default;

    void setWorldPosition(double x, double y);

    [[nodiscard]] double getWorldX() const;

    [[nodiscard]] double getWorldY() const;

    [[nodiscard]] float getBaseHeight() const;

    [[nodiscard]] int getHp() const;

    [[nodiscard]] int getMaxHp() const;

    [[nodiscard]] bool isDead() const;

    void takeDamage(int amount);

    virtual void update(double deltaTime, const Map &map, const Player &player);

    // void setTypeCode(int code) { typeCode_ = code; }

    [[nodiscard]] virtual double attackRange() const = 0;

    [[nodiscard]] virtual int attackDamage() const = 0;

    [[nodiscard]] virtual const char *typeName() const = 0;

    virtual void attackPlayer(Player &player, double deltaTime, const Map &map) = 0;

    //virtual void SRAttackPlayer(Player& player, double deltaTime) {} ;
};

#endif // OOP_ENEMY_H
