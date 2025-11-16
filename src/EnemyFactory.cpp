#include "EnemyFactory.h++"

#include "SREnemy.h++"
#include "LREnemy.h++"
#include "HDEnemy.h++"

#include <iostream>

std::unique_ptr<Enemy> EnemyFactory::createEnemy(EnemyType type, double x, double y) {
    
    switch (type) {
        case EnemyType::ShortRange: {
            auto e = std::make_unique<ShortRangeEnemy>(x, y);
            std::cout << *e << "\n";
            return e;
        }

        case EnemyType::LongRange: {
            auto e = std::make_unique<LongRangeEnemy>(x, y);
            std::cout << *e << "\n";
            return e;
        }

        case EnemyType::HighDamage: {
            auto e = std::make_unique<LongRangeHighDamageEnemy>(x, y);
            std::cout << *e << "\n";
            return e;
        }

        default:
            return nullptr;
    }
}