//
// Created by dlex on 30.11.2025.
//

#ifndef OOP_PICKUPSTRATEGIES_H
#define OOP_PICKUPSTRATEGIES_H
#include "Player.h++"
#include <iostream>
#include "GameManager.h++"
#include <string>

class HealthStrategy {
public :
    static void apply(Player &p) {
        p.heal(250);
    }

    static std::string getTextureName() { return "Medkit"; }
};

class AmmoStrategy {
public :
    static void apply(Player &p) {
        p.addAmmo(50);
    }

    static std::string getTextureName() { return "Ammo"; }
};

class MixedStrategy {
public :
    static void apply(Player &p) {
        p.heal(150);
        p.addAmmo(25);
    }
    static std::string getTextureName() { return "Mixed"; }
};

class PointsStrategy {
public:
    static void apply(Player &p) {
        p.addPoints(100);
    }

    static std::string getTextureName() { return "Points"; }
};
#endif //OOP_PICKUPSTRATEGIES_H
