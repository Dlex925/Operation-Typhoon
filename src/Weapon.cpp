#include "Weapon.h++"
#include "Enemy.h++"
#include "Map.h++"
#include "Player.h++"
#include <cmath>
#include <limits>
#include <algorithm>

Weapon::Weapon()
    : texture1("assets/Weapon1.png"), texture2("assets/Weapon2.png"), knifeTexture("assets/Cutit.png"),
      sprite(texture1) {
    float scale = 15.75f;
    sprite.setPosition({800, 450});
    sprite.setScale({scale, scale});
        originalY = 450;
}

void Weapon::update(float deltaTime) {
    if (shooting) {
        shootTimer += deltaTime;
        if (shootTimer >= shootDuration) {
            shooting = false;
            shootTimer = 0.0f;
            sprite.setTexture(texture1, true);
        }
    }
}


int Weapon::hitscanClosestEnemy(const Map &map, const Player &player,
                                const std::vector<std::unique_ptr<Enemy>> &enemies,
                                double rangeCap, double hitWidth) {
    if (enemies.empty()) return -1;

    double rayDirX = player.getDirX();
    double rayDirY = player.getDirY();

    int mapX = static_cast<int>(player.getX());
    int mapY = static_cast<int>(player.getY());

    double deltaDistX = (rayDirX == 0) ? 1e30 : std::abs(1 / rayDirX);
    double deltaDistY = (rayDirY == 0) ? 1e30 : std::abs(1 / rayDirY);

    double sideDistX, sideDistY;
    int stepX, stepY;

    if (rayDirX < 0) {
        stepX = -1;
        sideDistX = (player.getX() - mapX) * deltaDistX;
    } else {
        stepX = 1;
        sideDistX = (static_cast<double>(mapX) + 1.0 - player.getX()) * deltaDistX;
    }
    if (rayDirY < 0) {
        stepY = -1;
        sideDistY = (player.getY() - mapY) * deltaDistY;
    } else {
        stepY = 1;
        sideDistY = (static_cast<double>(mapY) + 1.0 - player.getY()) * deltaDistY;
    }

    int hit = 0, side = 0;
    int safeCounter = 0;
    while (hit == 0 && safeCounter < 4096) {
        ++safeCounter;
        if (sideDistX < sideDistY) {
            sideDistX += deltaDistX;
            mapX += stepX;
            side = 0;
        } else {
            sideDistY += deltaDistY;
            mapY += stepY;
            side = 1;
        }
        if (map.isWall(static_cast<double>(mapX), static_cast<double>(mapY))) hit = 1;
    }
    double wallDist = (side == 0) ? (sideDistX - deltaDistX) : (sideDistY - deltaDistY);

    double effectiveMaxDist = std::min(wallDist, rangeCap);

    int closestIdx = -1;
    double closestForward = std::numeric_limits<double>::infinity();

    for (size_t i = 0; i < enemies.size(); ++i) {
        const auto &ptr = enemies[i];
        if (!ptr) continue;
        const Enemy &e = *ptr;
        if (e.isDead()) continue;
        double vx = e.getWorldX() - player.getX();
        double vy = e.getWorldY() - player.getY();
        double forward = vx * rayDirX + vy * rayDirY;
        if (forward <= 0) continue;
        double perp = std::abs(vx * (-rayDirY) + vy * rayDirX);
        if (perp > hitWidth) continue;
        if (forward < effectiveMaxDist && forward < closestForward) {
            closestForward = forward;
            closestIdx = static_cast<int>(i);
        }
    }

    return closestIdx;
}

bool Weapon::shootPrimary(const Map &map, const Player &player, std::vector<std::unique_ptr<Enemy>> &enemies) {
    bool started = false;
    if (!shooting) {
        shooting = true;
        shootTimer = 0.0f;
        sprite.setTexture(texture2, true);
        started = true;
    }

    if (started) {
        int idx = hitscanClosestEnemy(map, player, enemies, std::numeric_limits<double>::infinity(), 0.3);
        if (idx >= 0) {
            if (enemies[static_cast<size_t>(idx)]) {
                enemies[static_cast<size_t>(idx)]->takeDamage(50);
            }
        }
    }
    return started;
}

bool Weapon::shootSecondary(const Map &map, const Player &player, std::vector<std::unique_ptr<Enemy>> &enemies) {
    bool started = false;
    if (!shooting) {
        shooting = true;
        shootTimer = 0.0f;
        sprite.setTexture(knifeTexture, true);
        started = true;
    }

    if (started) {
        int idx = hitscanClosestEnemy(map, player, enemies, 3.0, 0.3);
        if (idx >= 0) {
            if (enemies[static_cast<size_t>(idx)]) {
                enemies[static_cast<size_t>(idx)]->takeDamage(100);
            }
        }
    }
    return started;
}

void Weapon::draw(sf::RenderWindow &window) const {
    sf::Sprite spr = sprite;

    spr.setScale({1.f, 1.f});
    spr.setRotation(sf::degrees(0));
    spr.setPosition({0.f, 0.f});
    spr.setOrigin({0.f, 0.f});

    auto lb = spr.getLocalBounds();
    float baseW = lb.size.x;
    float baseH = lb.size.y;
    if (baseW <= 0.f || baseH <= 0.f) {
        window.draw(spr);
        return;
    }

    auto viewSize = window.getView().getSize();
    float winW = viewSize.x;
    float winH = viewSize.y;

    float targetH = std::max(100.f, winH * 0.33f);
    float scale = (targetH / baseH) * 2.0f;
    spr.setScale({scale, scale});

    spr.setOrigin({lb.position.x + lb.size.x * 0.5f - 5, lb.position.y + lb.size.y});

    float bottomMargin = std::max(2.f, winH * 0.005f);
    spr.setPosition({winW * 0.5f, winH - bottomMargin});

    window.draw(spr);
}

std::ostream &operator<<(std::ostream &os, const Weapon &weapon) {
    os << "\nWeapon\n" << weapon.shootDuration << " - shootDuration\n" << weapon.shootTimer << " - shootTimer\n" <<
            weapon.shooting << " is Shooting\n";
    return os ;
 }
