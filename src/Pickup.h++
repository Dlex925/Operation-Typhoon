//
// Created by dlex on 30.11.2025.
//

#ifndef OOP_PICKUP_H
#define OOP_PICKUP_H

#include "Player.h++"
#include <string>
#include <memory>

class PickupBase {
protected:
    double x, y;
    bool active = true;

public:
    PickupBase(double x, double y);

    virtual ~PickupBase() = default;

    virtual void updateAndCollect(Player &p) = 0;

    [[nodiscard]] double getX() const { return x; }
    [[nodiscard]] double getY() const { return y; }
    [[nodiscard]] bool isActive() const { return active; }

    [[nodiscard]] virtual std::string getTextureType() const = 0;
};

template<typename TStrategy>
class PickupItem : public PickupBase {
public:
    PickupItem(double x, double y) : PickupBase(x, y) {
    }

    void updateAndCollect(Player &p) override {
        if (!active) return;

        double dx = x - p.getX();
        double dy = y - p.getY();
        double distSq = dx * dx + dy * dy;

        if (distSq < 0.25) {
            TStrategy::apply(p);
            active = false;
        }
    }

    [[nodiscard]] std::string getTextureType() const override {
        return TStrategy::getTextureName();
    }
};


#endif //OOP_PICKUP_H
