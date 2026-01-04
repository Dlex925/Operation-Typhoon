#include "GameManager.h++"
#include <SFML/Audio.hpp>
#include <optional>
#include <cmath>
#include <algorithm>
#include <memory>
#include "SREnemy.h++"
#include "EnemyFactory.h++"
#include "EnemyType.h++"
#include "Menu.h++"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

GameManager::GameManager(sf::RenderWindow& win, std::string harta, Player& p)
    : window(win), map(harta, initialPlayerX_, initialPlayerY_), player(p), raycast(window, map, player) {
    std::cout << initialPlayerX_ << ", " << initialPlayerY_ << "\n";

    curentMapIndex_ = 1;

    countMaps();
    std::cout << "Detected maps: " << maxMaps_ << "\n";

    player.setPosition(initialPlayerX_, initialPlayerY_);
    spawnEnemiesFromMap();
    spawnPickupsFromMap();
}

void GameManager::countMaps() {
    maxMaps_ = 0;
    if (fs::exists("assets")) {
        for (const auto &entry: fs::directory_iterator("assets")) {
            if (entry.is_regular_file()) {
                std::string filename = entry.path().filename().string();

                if (filename.starts_with("map")) {
                    if (filename == "map_created" || filename == "map_random") {
                        continue;
                    }

                    maxMaps_++;
                }
            }
        }
    } else {
        std::cerr << "Warning: 'assets' folder not found. Defaulting to 1 map.\n";
        maxMaps_ = 1;
    }

    if (maxMaps_ == 0) maxMaps_ = 1;
}
void GameManager::spawnEnemiesFromMap() {
    enemies.clear();
    for (unsigned long y = 0; y < map.getHeight(); ++y) {
        for (unsigned long x = 0; x < map.getWidth(); ++x) {
            int cell = map.getCell(x, y);

            double cx = static_cast<double>(x) + 0.5;
            double cy = static_cast<double>(y) + 0.5;

            EnemyType type = static_cast<EnemyType>(cell);

            auto newEnemy = EnemyFactory::createEnemy(type, cx, cy);

            if (newEnemy) {
                enemies.push_back(std::move(newEnemy));
            }
        }
    }
}
void GameManager::spawnPickupsFromMap() {
    pickups.clear();
    for (unsigned long y = 0; y < map.getHeight(); ++y) {
        for (unsigned long x = 0; x < map.getWidth(); ++x) {
            int cell = map.getCell(x, y);
            EnemyType type = static_cast<EnemyType>(cell);
            double cx = static_cast<double>(x) + 0.5;
            double cy = static_cast<double>(y) + 0.5;
            auto item = EnemyFactory::createPickup(type, cx, cy);
            if (item) {
                pickups.push_back(std::move(item));
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
    player.addPoints(-9999999);

    if (gameOverJingle_.getStatus() == sf::SoundSource::Status::Playing) {
        gameOverJingle_.stop();
    }

    if (music.getStatus() == sf::SoundSource::Status::Paused) {
        music.play();
    }

    curentMapIndex_ = 1;
    std::string firstMap = "assets/map1";
    map = Map(firstMap, initialPlayerX_, initialPlayerY_);

    player.resetForNewGame();
    player.setPosition(initialPlayerX_, initialPlayerY_);

    spawnEnemiesFromMap();
    spawnPickupsFromMap();

    window.setMouseCursorVisible(false);
    auto viewSize = window.getView().getSize();
    int winW = viewSize.x;
    int winH = viewSize.y;
    sf::Mouse::setPosition({winW / 2, winH / 2}, window);
}

void GameManager::start() {
    sf::Clock clock;
    if (player.getMaxHp() < player.getHp())
        throw GameException("Please give the player less hp then max hp");

    window.setMouseCursorVisible(false);
    auto viewSize = window.getView().getSize();
    sf::Mouse::setPosition({static_cast<int>(viewSize.x) / 2, static_cast<int>(viewSize.y) / 2}, window);

    if (!uiFont.openFromFile("assets/0xProtoNerdFontPropo-Regular.ttf")) {
        throw FontException("assets/0xProtoNerdFontPropo-Regular.ttf");
    }
    uiFontLoaded_ = true;

    if (music.openFromFile("assets/ezoteric.ogg")) {
        music.setLooping(true);
        music.play();
    } else throw AssetLoadException("assets/ezoteric.ogg", "Music");

    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        handleInput(deltaTime);

        if (gameOver_) {
            gameOverTimer_ += deltaTime;
            if (gameOverTimer_ > 3.0f) {
                music.stop();
                if (gameOverJingle_.getStatus() == sf::SoundSource::Status::Playing)
                    gameOverJingle_.stop();

                std::string name = Menu::getInstance().askName(window, player.score_());
                Menu::getInstance().saveHighscore(name, player.score_());
                Menu::getInstance().showHighscores(window);

                window.close();
                return;
            }
        } else if (levelComplete_) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
                winmusic.stop();

                if (gameFin) {
                    music.stop();
                    std::string name = Menu::getInstance().askName(window, player.score_());
                    Menu::getInstance().saveHighscore(name, player.score_());
                    Menu::getInstance().showHighscores(window);

                    window.close();
                    return;
                } else {
                    loadNextLevel();
                }
            }
        } else if (!paused_) {
            player.updateWeapon(deltaTime);
            for (auto &e: enemies) {
                if (!e) continue;
                if (!e->isDead()) {
                    e->update(deltaTime, map, player);
                    e->attackPlayer(player, deltaTime, map);
                } else {
                    e->updateDeathTimer(deltaTime);
                }
            }

            enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
                                         [](const std::unique_ptr<Enemy> &e) {
                                             return !e || e->shouldDespawn();
                                         }), enemies.end());


            for (auto &item: pickups) {
                item->updateAndCollect(player);
            }

            if (!levelComplete_) {
                bool allDead = true;
                for (const auto &e : enemies) {
                    if (e && !e->isDead()) {
                        allDead = false;
                        break;
                    }
                }

                if (allDead) {
                    levelComplete_ = true;
                    if (curentMapIndex_ >= maxMaps_) {
                        gameFin = true;
                    }

                    music.pause();
                    if (winmusic.openFromFile("assets/win.ogg")) {
                        winmusic.setLooping(false);
                        winmusic.play();
                    }
                }
            }

            if (player.getHp() <= 0) {
                gameOver_ = true;
                gameOverTimer_ = 0.f;
                if (music.getStatus() == sf::SoundSource::Status::Playing) music.pause();
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
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)) {
        restartGame_();
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

    if (paused_) return;

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
    struct Sprites {
        double dist;
        int type; // 0 = Enemy, 1 = Pickup
        int index;
    };

    std::vector<Sprites> allSprites;
    for (size_t i = 0; i < enemies.size(); ++i) {
        if (enemies[i]) {
            double dx = player.getX() - enemies[i]->getWorldX();
            double dy = player.getY() - enemies[i]->getWorldY();
            double dist = dx * dx + dy * dy;
            allSprites.push_back({dist, 0, static_cast<int>(i)});
        }
    }
    for (size_t i = 0; i < pickups.size(); ++i) {
        if (pickups[i]->isActive()) {
            double dx = player.getX() - pickups[i]->getX();
            double dy = player.getY() - pickups[i]->getY();
            double dist = dx * dx + dy * dy;
            allSprites.push_back({dist, 1, static_cast<int>(i)});
        }
    }
    std::sort(allSprites.begin(), allSprites.end(), [](const Sprites &a, const Sprites &b) {
        return a.dist > b.dist;
    });

    for (const auto &sprite: allSprites) {
        if (sprite.type == 0) {
            raycast.renderEnemy(*enemies[sprite.index]);
        } else {
            raycast.renderPickup(*pickups[sprite.index]);
        }
    }
    player.drawWeapon(window);
    if (uiFontLoaded_) {
        sf::Text hud(uiFont, "");
        hud.setCharacterSize(55);
        hud.setFillColor(sf::Color(0, 255, 100));
        hud.setString("HP: " + std::to_string(player.getHp()) + "/" + std::to_string(player.getMaxHp()) +
                      "    Ammo: " + std::to_string(player.getAmmo()) +
                      "    Score: " + std::to_string(player.score_()));
        hud.setPosition({12.f, 10.f});
        window.draw(hud);
    } else {
        throw FontException("assets/0xProtoNerdFontPropo-Regular.ttf");
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
    if (levelComplete_) {
        sf::RectangleShape overlay({static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)});
        overlay.setFillColor(sf::Color(0, 0, 0, 150));
        window.draw(overlay);

        if (uiFontLoaded_) {
            std::string wintxt;
            if (gameFin) {
                wintxt = "THANKS FOR PLAYING";
            } else {
                wintxt = "Level Complete";
            }

            sf::Text winText(uiFont, wintxt);
            winText.setCharacterSize(88);
            winText.setFillColor(sf::Color::Red);

            auto b = winText.getLocalBounds();
            winText.setPosition({(window.getSize().x - b.size.x) / 2.f, window.getSize().y * 0.3f});
            window.draw(winText);

            std::string nexttxt;
            if (gameFin) {
                nexttxt = "Press Space to Finish";
            } else {
                nexttxt = "Press Space for the next level";
            }

            sf::Text nextText(uiFont, nexttxt);
            nextText.setCharacterSize(88);
            nextText.setFillColor(sf::Color::Red);

            auto nb = nextText.getLocalBounds();
            nextText.setPosition({(window.getSize().x - nb.size.x) / 2.f, window.getSize().y * 0.5f});
            window.draw(nextText);
        }
    }


    window.display();
}

void GameManager::loadNextLevel() {
    curentMapIndex_++;

    std::string nextMapPath = "assets/map" + std::to_string(curentMapIndex_);

    if (!fs::exists(nextMapPath)) {
        gameFin = true;
        levelComplete_ = true;
        return;
    }

    try {
        map = Map(nextMapPath, initialPlayerX_, initialPlayerY_);
    } catch (...) {
        gameFin = true;
        levelComplete_ = true;
        return;
    }

    player.setPosition(initialPlayerX_, initialPlayerY_);

    enemies.clear();
    spawnEnemiesFromMap();
    pickups.clear();
    spawnPickupsFromMap();

    levelComplete_ = false;

    if (music.getStatus() != sf::SoundSource::Status::Playing) {
        music.play();
    }
}
