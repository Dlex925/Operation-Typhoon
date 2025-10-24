#ifndef OOP_ENEMY_H
#define OOP_ENEMY_H

class Map;
class Player;

class Enemy {
    double worldX{2.5};
    double worldY{2.5};
    float baseHeight{1.2f};
    double moveSpeed{2.0};
    double stopDistance{0.75};
    int maxHp{200};
    int hp{200};
    bool dead{false};

public:
    explicit Enemy(double x = 2.5, double y = 2.5, float heightScale = 0.9f);

    void setWorldPosition(double x, double y);

    [[nodiscard]] double getWorldX() const;

    [[nodiscard]] double getWorldY() const;

    [[nodiscard]] float getBaseHeight() const;

    [[nodiscard]] int getHp() const;

    [[nodiscard]] int getMaxHp() const;

    [[nodiscard]] bool isDead() const;

    void takeDamage(int amount);

    void update(double deltaTime, const Map &map, const Player &player);
};

#endif // OOP_ENEMY_H
