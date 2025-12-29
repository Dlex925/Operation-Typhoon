#include "EnemyFactory.h++"

#include "SREnemy.h++"
#include "LREnemy.h++"
#include "HDEnemy.h++"

#include <iostream>

#include "Boom.h++"
#include "PickupStrategies.h++"


std::unique_ptr<Enemy> EnemyFactory::createEnemy(EnemyType type, double x, double y) {
    std::unique_ptr<Enemy> enemy = nullptr;

    switch (type) {
        case EnemyType::ShortRange:
            enemy = std::make_unique<ShortRangeEnemy>(x, y);
            break;

        case EnemyType::LongRange:
            enemy = std::make_unique<LongRangeEnemy>(x, y);
            break;

        case EnemyType::HighDamage:
            enemy = std::make_unique<LongRangeHighDamageEnemy>(x, y);
            break;

        case EnemyType::Boom:
            enemy = std::make_unique<Boom>(x, y);
            break;

        case EnemyType::None:
        case EnemyType::Wall:
        case EnemyType::Ammo:
        case EnemyType::Health:
        case EnemyType::Mixed:
        case EnemyType::Points:
            break;

        default:
            throw UnknownEnemyTypeException(static_cast<int>(type));
    }

    if (enemy) {
        std::cout << *enemy << "\n";
    }

    return enemy;
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
        case EnemyType::Points:
            return spawnPickup<PointsStrategy>(x, y);
        default:
            return nullptr;
    }
}
