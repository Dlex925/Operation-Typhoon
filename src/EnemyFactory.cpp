#include "EnemyFactory.h++"

#include "SREnemy.h++"
#include "LREnemy.h++"
#include "HDEnemy.h++"

#include <iostream>

#include "Boom.h++"

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
        case EnemyType::Boom: {
            auto e = std::make_unique<Boom>(x, y);
            std::cout << *e << "\n";
            return e;
        }
        case EnemyType::None:
        case EnemyType::Wall:
            return nullptr;
        default:
            throw UnknownEnemyTypeException(static_cast<int>(type));
    }
}
