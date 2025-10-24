#include "GameManager.h++"
#include <SFML/Audio.hpp>
#include <optional>
#include <cmath>
#include <algorithm>

GameManager::GameManager(sf::RenderWindow& win, std::string harta, Player& p)
    : window(win), map(harta), player(p), raycast(window, map, player) {
    // For testing - spawning enemies
    for (int i = 0; i <= 5; ++i) {
        for (int j = 0; j <= 5; ++j) {
            int ex = i * j, ey = j * j;
            if (!map.isWall(ex, ey)) {
                Enemy e;
                e.setWorldPosition(ex, ey);
                enemies.push_back(e);
            }
        }
    }
}

void GameManager::start() {
    sf::Clock clock;
    window.setMouseCursorVisible(false);
    sf::Mouse::setPosition({400, 300}, window);
    sf::Music music;
    if (!music.openFromFile("assets/ezoteric.ogg"))
        throw std::runtime_error("Failed to load music file: assets/ezoteric.ogg");
    music.play();

    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        handleInput(deltaTime);
        player.updateWeapon(deltaTime);
        for (auto &e: enemies) {
            e.update(deltaTime, map, player);
        }
        Engine();
    }
}

void GameManager::handleInput(float deltaTime) {
    while (std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
    }

    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    if (mousePos.x != 400) {
        player.RotateCamera(static_cast<float>(400 - mousePos.x));
        sf::Mouse::setPosition({400, 300}, window);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) player.MoveForward(deltaTime, map);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) player.MoveBackward(deltaTime, map);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) player.MoveLeft(deltaTime, map);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) player.MoveRight(deltaTime, map);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) window.close();
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
        if (player.getWeapon().shootPrimary(map, player, enemies)) {
            performHitscanDamage();
        }
    }
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
        if (player.getWeapon().shootSecondary(map, player, enemies)) {
            performShortRangeAttackDamage();
        }
    }
}

void GameManager::Engine() const{
    window.clear(sf::Color(0, 0, 0));
    raycast.render();

    std::vector<size_t> order(enemies.size());
    for (size_t i = 0; i < enemies.size(); ++i) order[i] = i;
    std::sort(order.begin(), order.end(), [&](size_t a, size_t b) {
        double dxA = enemies[a].getWorldX() - player.getX();
        double dyA = enemies[a].getWorldY() - player.getY();
        double dxB = enemies[b].getWorldX() - player.getX();
        double dyB = enemies[b].getWorldY() - player.getY();
        double da = dxA * dxA + dyA * dyA;
        double db = dxB * dxB + dyB * dyB;
        return da > db;
    });
    for (size_t idx: order) {
        raycast.renderEnemy(enemies[idx]);
    }
    player.drawWeapon(window);
    window.display();
}


void GameManager::performHitscanDamage() {
    if (enemies.empty()) return;
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
    while (hit == 0) {
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
        if (std::abs(mapX) > 1000 || std::abs(mapY) > 1000) { hit = 1; }
    }
    double wallDist = (side == 0) ? (sideDistX - deltaDistX) : (sideDistY - deltaDistY);

    const double hitWidth = 0.3;
    int closestIdx = -1;
    double closestForward = 1e30;

    for (size_t i = 0; i < enemies.size(); ++i) {
        const Enemy &e = enemies[i];
        if (e.isDead()) continue;
        double vx = e.getWorldX() - player.getX();
        double vy = e.getWorldY() - player.getY();
        double forward = vx * rayDirX + vy * rayDirY;
        if (forward <= 0) continue;
        double perp = std::abs(vx * (-rayDirY) + vy * rayDirX);
        if (perp > hitWidth) continue;
        if (forward < wallDist && forward < closestForward) {
            closestForward = forward;
            closestIdx = static_cast<int>(i);
        }
    }

    if (closestIdx >= 0) {
        enemies[static_cast<size_t>(closestIdx)].takeDamage(50);
    }
}

void GameManager::performShortRangeAttackDamage() {
    if (enemies.empty()) return;
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
    while (hit == 0) {
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
        if (std::abs(mapX) > 1000 || std::abs(mapY) > 1000) { hit = 1; }
    }
    double wallDist = (side == 0) ? (sideDistX - deltaDistX) : (sideDistY - deltaDistY);

    const double rangeCap = 3.0;
    double effectiveMaxDist = std::min(wallDist, rangeCap);

    const double hitWidth = 0.3;
    int closestIdx = -1;
    double closestForward = 1e30;

    for (size_t i = 0; i < enemies.size(); ++i) {
        const Enemy &e = enemies[i];
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

    if (closestIdx >= 0) {
        enemies[static_cast<size_t>(closestIdx)].takeDamage(100);
    }
}