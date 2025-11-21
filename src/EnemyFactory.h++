//
// Created by dlex on 12.11.2025.
//

#ifndef OOP_ENEMYFACTORY_H
#define OOP_ENEMYFACTORY_H
#include <memory>
#include "Enemy.h++"
#include "EnemyType.h++"
#include "Exceptions.h++"

class EnemyFactory {
public:
    static std::unique_ptr<Enemy> createEnemy(EnemyType type, double x, double y);
};


#endif //OOP_ENEMYFACTORY_H
