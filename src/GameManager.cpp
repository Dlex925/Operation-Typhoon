#include "GameManager.h++"
#include <SFML/Audio.hpp>
#include <optional>
#include <cmath>
#include <algorithm>
#include <memory>
#include "SREnemy.h++"
#include "LREnemy.h++"
#include "HDEnemy.h++"

GameManager::GameManager(sf::RenderWindow& win, std::string harta, Player& p)
    : window(win), map(harta), player(p), raycast(window, map, player) {
    initialPlayerX_ = player.getX();
    initialPlayerY_ = player.getY();
    spawnEnemiesFromMap();
}

void GameManager::spawnEnemiesFromMap() {
    enemies.clear();
    for (unsigned long y = 0; y < map.getHeight(); ++y) {
        for (unsigned long x = 0; x < map.getWidth(); ++x) {
            int cell = map.getCell(x, y);
            double cx = static_cast<double>(x) + 0.5;
            double cy = static_cast<double>(y) + 0.5;
            if (cell == 2) {
                auto e = std::make_unique<ShortRangeEnemy>(cx, cy);
                enemies.push_back(std::move(e));
            } else if (cell == 3) {
                auto e = std::make_unique<LongRangeEnemy>(cx, cy);
                enemies.push_back(std::move(e));
            } else if (cell == 4) {
                auto e = std::make_unique<LongRangeHighDamageEnemy>(cx, cy);
                enemies.push_back(std::move(e));
            }
        }
    }
}

void GameManager::restartGame_() {
    gameOver_ = false;
    gameOverTimer_ = 0.f;
    gameOverSoundPlayed_ = false;
    paused_ = false;
    pHeld_ = false;
    rHeld_ = false;

    if (gameOverJingle_.getStatus() == sf::SoundSource::Status::Playing) {
        gameOverJingle_.stop();
    }

    if (music.getStatus() == sf::SoundSource::Status::Paused) {
        music.play();
    }

    player.resetForNewGame();
    player.setPosition(initialPlayerX_, initialPlayerY_);

    spawnEnemiesFromMap();

    window.setMouseCursorVisible(false);
    auto viewSize = window.getView().getSize();
    int winW = viewSize.x;
    int winH = viewSize.y;
    sf::Mouse::setPosition({winW / 2, winH / 2}, window);
}

void GameManager::start() {
    sf::Clock clock;
    window.setMouseCursorVisible(false);
    auto viewSize = window.getView().getSize();
    int winW = viewSize.x;
    int winH = viewSize.y;
    sf::Mouse::setPosition({winW / 2, winH / 2}, window);

    uiFontLoaded_ = uiFont.openFromFile("assets/0xProtoNerdFontPropo-Regular.ttf");

    if (music.openFromFile("assets/ezoteric.ogg")) {
        music.setLooping(true);
        music.play();
    }

    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        handleInput(deltaTime);

        if (gameOver_) {
            gameOverTimer_ += deltaTime;
        } else if (!paused_) {
            player.updateWeapon(deltaTime);
            for (auto &e: enemies) {
                if (!e) continue;
                if (!e->isDead()) {
                    e->update(deltaTime, map, player);
                    e->attackPlayer(player, deltaTime, map);
                }
            }
            if (player.getHp() <= 0) {
                gameOver_ = true;
                gameOverTimer_ = 0.f;
                if (music.getStatus() == sf::SoundSource::Status::Playing) {
                    music.pause();
                }
                if (!gameOverSoundPlayed_) {
                    if (gameOverJingle_.openFromFile("assets/Dead.ogg")) {
                        gameOverJingle_.setLooping(false);
                        gameOverJingle_.play();
                    }
                    gameOverSoundPlayed_ = true;
                }
            }
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

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
        window.close();
        return;
    }

    if (gameOver_) {
        bool rNow = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R);
        if (rNow && !rHeld_) {
            restartGame_();
        }
        rHeld_ = rNow;
        return;
    }

    bool pNow = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P);
    if (pNow && !pHeld_) {
        paused_ = !paused_;
        if (music.getStatus() == sf::SoundSource::Status::Playing && paused_) {
            music.pause();
        } else if (music.getStatus() == sf::SoundSource::Status::Paused && !paused_) {
            music.play();
        }
    }
    pHeld_ = pNow;

    if (paused_) return;//

    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    if (mousePos.x != 400) {
        player.RotateCamera(static_cast<float>(400 - mousePos.x));
        sf::Mouse::setPosition({400, 300}, window);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) player.MoveForward(deltaTime, map);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) player.MoveBackward(deltaTime, map);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) player.MoveLeft(deltaTime, map);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) player.MoveRight(deltaTime, map);

    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
        if (player.getAmmo() > 0) {
            if (player.getWeapon().shootPrimary(map, player, enemies)) {
                player.consumeAmmo(1);
            }
        }
    }
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
        player.getWeapon().shootSecondary(map, player, enemies);
    }
}

void GameManager::Engine() const{
    window.clear(sf::Color(0, 0, 0));
    raycast.render();

    std::vector<size_t> order(enemies.size());
    for (size_t i = 0; i < enemies.size(); ++i) order[i] = i;
    std::sort(order.begin(), order.end(), [&](size_t a, size_t b) {
        const Enemy *ea = enemies[a].get();
        const Enemy *eb = enemies[b].get();
        double dxA = ea ? (ea->getWorldX() - player.getX()) : 0.0;
        double dyA = ea ? (ea->getWorldY() - player.getY()) : 0.0;
        double dxB = eb ? (eb->getWorldX() - player.getX()) : 0.0;
        double dyB = eb ? (eb->getWorldY() - player.getY()) : 0.0;
        double da = dxA * dxA + dyA * dyA;
        double db = dxB * dxB + dyB * dyB;
        return da > db;
    });
    for (size_t idx: order) {
        if (enemies[idx]) raycast.renderEnemy(*enemies[idx]);
    }
    player.drawWeapon(window);

    if (uiFontLoaded_) {
        sf::Text hud(uiFont, "");
        hud.setCharacterSize(22);
        hud.setFillColor(sf::Color(110, 0, 100));
        hud.setString("HP: " + std::to_string(player.getHp()) + "/" + std::to_string(player.getMaxHp()) +
                      "    Ammo: " + std::to_string(player.getAmmo()));
        hud.setPosition({12.f, 10.f});
        window.draw(hud);
    }

    if (paused_) {
        sf::RectangleShape overlay({static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)});
        overlay.setFillColor(sf::Color(0, 0, 0, 160));
        window.draw(overlay);

        if (uiFontLoaded_) {
            sf::Text pausedText(uiFont, "PAUSED");
            pausedText.setCharacterSize(72);
            pausedText.setFillColor(sf::Color(255, 255, 255));
            auto b = pausedText.getLocalBounds();
            float x = (window.getSize().x - b.size.x) * 0.5f - b.position.x;
            float y = (window.getSize().y - b.size.y) * 0.4f - b.position.y;
            pausedText.setPosition({x, y});
            window.draw(pausedText);
        }
    }

    if (gameOver_) {
        const float duration = 2.5f;
        float t = std::min(gameOverTimer_, duration);
        float progress = (t / duration);
        float h = static_cast<float>(window.getSize().y);
        float w = static_cast<float>(window.getSize().x);
        float filledH = h * progress;

        sf::RectangleShape redPart({w, filledH});
        redPart.setPosition({0.f, 0.f});
        redPart.setFillColor(sf::Color(180, 0, 0, 255));
        window.draw(redPart);

        sf::RectangleShape tint({w, h - filledH});
        tint.setPosition({0.f, filledH});
        unsigned char alpha = static_cast<unsigned char>(std::clamp(progress, 0.f, 1.f) * 200.f);
        tint.setFillColor(sf::Color(120, 0, 0, alpha));
        window.draw(tint);
        if (uiFontLoaded_) {
            sf::Text go(uiFont, "GAME OVER");
            go.setCharacterSize(88);
            go.setFillColor(sf::Color(255, 230, 230));
            auto b = go.getLocalBounds();
            float x = (w - b.size.x) * 0.5f - b.position.x;
            float y = (h - b.size.y) * 0.35f - b.position.y;
            go.setPosition({x, y});
            window.draw(go);
            sf::Text hint(uiFont, "Press R to restart or Esc to quit");
            hint.setCharacterSize(28);
            hint.setFillColor(sf::Color(255, 240, 240));
            auto hb = hint.getLocalBounds();
            float hx = (w - hb.size.x) * 0.5f - hb.position.x;
            float hy = y + b.size.y + 40.f;
            hint.setPosition({hx, hy});
            window.draw(hint);
        }

    }

    window.display();
}



