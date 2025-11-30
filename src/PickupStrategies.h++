//
// Created by dlex on 30.11.2025.
//

#ifndef OOP_PICKUPSTRATEGIES_H
#define OOP_PICKUPSTRATEGIES_H
#include "Player.h++"
#include <iostream>
#include <string>

struct HealthStrategy {
    static void apply(Player &p) {
        p.heal(250);
    }

    static std::string getTextureName() { return "Medkit"; }
};

struct AmmoStrategy {
    static void apply(Player &p) {
        p.addAmmo(50);
    }

    static std::string getTextureName() { return "Ammo"; }
};

struct MixedStrategy {
    static void apply(Player &p) {
        p.heal(150);
        p.addAmmo(25);
    }

    static std::string getTextureName() { return "Mixed"; }
};
#endif //OOP_PICKUPSTRATEGIES_H
