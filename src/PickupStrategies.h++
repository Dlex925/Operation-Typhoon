//
// Created by dlex on 30.11.2025.
//

#ifndef OOP_PICKUPSTRATEGIES_H
#define OOP_PICKUPSTRATEGIES_H
#include "Player.h++"
#include <iostream>
#include <string>

struct HealthStrategy {
    static void apply(Player& p) {
        p.heal(50);
    }
    static std::string getTextureName() { return "Medkit"; }
};

struct AmmoStrategy {
    static void apply(Player& p) {
        p.addAmmo(20);
    }
    static std::string getTextureName() { return "Ammo"; }
};

struct MixedStrategy {
    static void apply(Player& p) {
        p.heal(25);
        p.addAmmo(10);
    }
    static std::string getTextureName() { return "Mixed"; }
};
#endif //OOP_PICKUPSTRATEGIES_H