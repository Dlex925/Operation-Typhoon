#include "EnemyFactory.h++"

#include "SREnemy.h++"
#include "LREnemy.h++"
#include "HDEnemy.h++"

#include <iostream>

#include "Boom.h++"
#include "PickupStrategies.h++"

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
        case EnemyType::Ammo:
        case EnemyType::Health:
        case EnemyType::Mixed:
        case EnemyType::Wall:
            return nullptr;
        default:
            throw UnknownEnemyTypeException(static_cast<int>(type));
    }
}
template<typename TStrategy>
std::unique_ptr<PickupBase> spawnPickup(double x, double y) {
    return std::make_unique<PickupItem<TStrategy> >(x, y);
}

std::unique_ptr<PickupBase> EnemyFactory::createPickup(EnemyType type, double x, double y) {
    switch (type) {
        case EnemyType::Health:
            return spawnPickup<HealthStrategy>(x, y);
        case EnemyType::Ammo:
            return spawnPickup<AmmoStrategy>(x, y);
        case EnemyType::Mixed:
            return spawnPickup<MixedStrategy>(x, y);
        default:
            return nullptr;
    }
}
