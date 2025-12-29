#ifndef OOP_ENEMY_H
#define OOP_ENEMY_H

#include <memory>
#include <string>

#include "Pathfinder.h++"
#include "Player.h++"
class Map;


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
    std::vector<Point> currentPath;
    float pathTimer = 0.0f;
    float deathTimer{0.0f};

protected:
    float attackInterval_{0.5f};
    float attackCooldown_{0.9f};
    static int enemy_no;
    static int enemy_dead;

    bool canAttack(const Player &player, const Map &map, double deltaTime);

    static bool hasLineOfSight(const Map &map, double sx, double sy, double tx, double ty);

public:
    explicit Enemy(double x = 2.5, double y = 2.5, float heightScale = 0.9f, int hp1 = 100, double mvspeed = 2.0);

    virtual ~Enemy() = default;

    void setWorldPosition(double x, double y);
    [[nodiscard]]
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

    [[nodiscard]] virtual const std::string typeName() const = 0;

    virtual void attackPlayer(Player &player, double deltaTime, const Map &map) = 0;

    [[nodiscard]] virtual std::unique_ptr<Enemy> clone() const = 0;

    void print(std::ostream &os) const {
        os << typeName() << "(hp=" << getHp() << "/" << getMaxHp() << ", x=" << getWorldX() << ", y=" << getWorldY() <<
                ")" << "\nDead_Enemies " << enemy_dead << "\nTotal Enemies " << enemy_no << '\n' << (
                    static_cast<float>(enemy_dead) / static_cast<float>(enemy_no) >= 0.5) << '\n' << static_cast<float>(
                    enemy_dead) / static_cast<float>(enemy_no) << "\n";
    }

    friend std::ostream &operator<<(std::ostream &os, const Enemy &e) {
        e.print(os);
        return os;
    }



    //virtual void SRAttackPlayer(Player& player, double deltaTime) {} ;
     [[nodiscard]] double Dist(const Player &player) const {
         double dx = getWorldX() - player.getX();
         double dy = getWorldY() - player.getY();
         double dist = dx * dx + dy * dy;
        return dist;
    }

    friend void swap(Enemy &lhs, Enemy &rhs) noexcept {
        using std::swap;
        swap(lhs.worldX, rhs.worldX);
        swap(lhs.worldY, rhs.worldY);
        swap(lhs.baseHeight, rhs.baseHeight);
        swap(lhs.moveSpeed, rhs.moveSpeed);
        swap(lhs.stopDistance, rhs.stopDistance);
        swap(lhs.maxHp, rhs.maxHp);
        swap(lhs.hp, rhs.hp);
        swap(lhs.dead, rhs.dead);
        swap(lhs.typeCode_, rhs.typeCode_);
        swap(lhs.attackInterval_, rhs.attackInterval_);
        swap(lhs.attackCooldown_, rhs.attackCooldown_);
    }

    void updateDeathTimer(float deltaTime) {
        if (dead) {
            deathTimer += deltaTime;
        }
    }

    [[nodiscard]] bool shouldDespawn() const {
        return deathTimer >= 10.0f;
    }
};

#endif // OOP_ENEMY_H
