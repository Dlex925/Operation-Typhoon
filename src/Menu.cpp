//
// Created by dlex on 06.11.2025.
//

#include "Menu.h++"
#include <algorithm>
#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <SFML/Audio.hpp>
#include <chrono>

static bool loadMenuFont(sf::Font &font) {
    const char *path = "assets/0xProtoNerdFontPropo-Regular.ttf";
    return font.openFromFile(path);
}

static std::string resolutionToString(const sf::Vector2u &v) {
    return std::to_string(v.x) + " x " + std::to_string(v.y);
}

Menu &Menu::getInstance() {
    static Menu instance;
    return instance;
}

sf::VideoMode Menu::selectResolution() {
    std::vector<sf::Vector2u> options = {
        {1280, 720}, {1600, 900}, {1920, 1080}, {2560, 1440}, {3840, 2160}
    };

    auto desktop = sf::VideoMode::getDesktopMode();
    sf::Vector2u deskSize{desktop.size.x, desktop.size.y};
    auto exists = std::find(options.begin(), options.end(), deskSize) != options.end();
    if (!exists) options.push_back(deskSize);

    std::size_t current = 0;

    sf::RenderWindow window(sf::VideoMode({1100, 450}), "Select Resolution");
    window.setVerticalSyncEnabled(true);

    sf::Font font;
    bool haveFont = loadMenuFont(font);

    std::unique_ptr<sf::Text> instructions;
    std::unique_ptr<sf::Text> selection;
    if (haveFont) {
        instructions = std::make_unique<sf::Text>(
            font,
            "Left/Right: resolution (windowed only)  |  Up/Down: toggle fullscreen/windowed  |  Enter: OK  Esc: cancel");
        selection = std::make_unique<sf::Text>(font, "");

        instructions->setCharacterSize(22);
        instructions->setFillColor(sf::Color::Yellow);
        instructions->setPosition({20.f, 18.f});

        selection->setCharacterSize(32);
        selection->setFillColor(sf::Color(255, 255, 255));
    }

    fullscreenChosen_ = false;

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            } else if (auto key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->code == sf::Keyboard::Key::Escape) {
                    window.close();
                    return sf::VideoMode({deskSize.x, deskSize.y});
                }
                if (key->code == sf::Keyboard::Key::Enter) {
                    sf::Music music;
                    if (music.openFromFile("assets/Close.ogg")) {
                        music.setLooping(false);
                        music.play();
                    }
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    if (fullscreenChosen_) {
                        auto dm = sf::VideoMode::getDesktopMode();
                        window.close();
                        return sf::VideoMode({dm.size.x, dm.size.y});
                    } else {
                        auto sel = options[current];
                        window.close();
                        return sf::VideoMode({sel.x, sel.y});
                    }
                }
                if (key->code == sf::Keyboard::Key::Right) {
                    if (!fullscreenChosen_) current = (current + 1) % options.size();
                }
                if (key->code == sf::Keyboard::Key::Left) {
                    if (!fullscreenChosen_) current = (current + options.size() - 1) % options.size();
                }
                if (key->code == sf::Keyboard::Key::Up) {
                    fullscreenChosen_ = true;
                }
                if (key->code == sf::Keyboard::Key::Down) {
                    fullscreenChosen_ = false;
                }
            }
        }

        window.clear(sf::Color(30, 30, 30));

        if (!fullscreenChosen_) {
            sf::CircleShape leftArrow(10.f, 3);
            leftArrow.setRotation(sf::degrees(90));
            leftArrow.setFillColor(sf::Color(180, 180, 180));
            leftArrow.setPosition({40.f, 150.f});
            window.draw(leftArrow);

            sf::CircleShape rightArrow(10.f, 3);
            rightArrow.setRotation(sf::degrees(270));
            rightArrow.setFillColor(sf::Color(180, 180, 180));
            rightArrow.setPosition({window.getSize().x - 40.f, 150.f});
            window.draw(rightArrow);
        }

        auto sel = options[current];
        //std::string modeStr = fullscreenChosen_ ? "Fullscreen" : "Windowed";

        if (fullscreenChosen_) {
            window.setTitle(
                std::string(
                    "Select Resolution | Mode: Fullscreen (desktop) | Up/Down to toggle, Enter to confirm, Esc to cancel")
            );
        } else {
            window.setTitle(
                std::string(
                    "Select Resolution | Mode: Windowed | Left/Right to change res, Enter to confirm, Esc to cancel | Selected: ")
                + resolutionToString(sel)
            );
        }

        if (haveFont) {
            if (fullscreenChosen_) {
                selection->setString("Mode: Fullscreen (desktop)");
            } else {
                selection->setString("Mode: Windowed    Resolution: " + resolutionToString(sel));
            }
            auto bounds = selection->getLocalBounds();
            float x = (window.getSize().x - bounds.size.x) * 0.5f - bounds.position.x;
            float y = 120.f - bounds.position.y;
            selection->setPosition({x, y});

            window.draw(*instructions);
            window.draw(*selection);
        }

        window.display();
    }

    auto sel = options[current];
    return sf::VideoMode({sel.x, sel.y});
}
int Menu::selectGameMode(sf::RenderWindow &window) {
    sf::Font font;
    if (!loadMenuFont(font)) return 1;
    sf::Text title(font, "SELECT MODE (press 1,2 or 3)", 50);
    title.setFillColor(sf::Color::Yellow);

    auto b = title.getLocalBounds();
    title.setPosition({
        (window.getSize().x - b.size.x) / 2.f,
        window.getSize().y * 0.2f
    });

    sf::Text opt1(font, "1. START MISSION", 35);
    sf::Text opt2(font, "2. MAP CREATOR", 35);
    sf::Text opt3(font, "3. GENERATE RANDOM MAP", 35);


    opt1.setPosition({
        (window.getSize().x - opt1.getLocalBounds().size.x) / 2.f,
        window.getSize().y * 0.45f
    });

    opt2.setPosition({
        (window.getSize().x - opt2.getLocalBounds().size.x) / 2.f,
        window.getSize().y * 0.55f
    });
    opt3.setPosition({
        (window.getSize().x - opt3.getLocalBounds().size.x) / 2.f,
        window.getSize().y * 0.65f
    });

    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
                return 0;
            }

            if (const auto *keyEvent = event->getIf<sf::Event::KeyPressed>()) {
                if (keyEvent->code == sf::Keyboard::Key::Num1) return 1;
                if (keyEvent->code == sf::Keyboard::Key::Num2) return 2;
                if (keyEvent->code == sf::Keyboard::Key::Escape) {
                    window.close();
                    return 0;
                }
                if (keyEvent->code == sf::Keyboard::Key::Num3) return 3;
            }
        }

        window.clear(sf::Color(30, 30, 30));
        window.draw(title);
        window.draw(opt1);
        window.draw(opt2);
        window.draw(opt3);
        window.display();
    }
    return 0;
}

std::pair<int, int> Menu::selectGenerationSize(sf::RenderWindow &window) {
    sf::Font font;
    if (!loadMenuFont(font)) return {30, 30};

    int w = 30;
    int h = 30;

    sf::Text title(font, "GENERATOR SETTINGS", 40);
    title.setFillColor(sf::Color::Yellow);
    sf::FloatRect tb = title.getLocalBounds();
    title.setPosition({(window.getSize().x - tb.size.x) / 2.f, 50.f});

    sf::Text info(font, "Use ARROWS to change size\nENTER to Generate\nESC to Cancel", 20);
    info.setFillColor(sf::Color::White);
    sf::FloatRect ib = info.getLocalBounds();
    info.setPosition({(window.getSize().x - ib.size.x) / 2.f, window.getSize().y - 100.f});

    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
                return {0, 0};
            }

            if (const auto *keyEvent = event->getIf<sf::Event::KeyPressed>()) {
                if (keyEvent->code == sf::Keyboard::Key::Escape) return {0, 0};
                if (keyEvent->code == sf::Keyboard::Key::Enter) return {w, h};

                if (keyEvent->code == sf::Keyboard::Key::Left) w = std::max(10, w - 2);
                if (keyEvent->code == sf::Keyboard::Key::Right) w = std::min(75, w + 2);
                if (keyEvent->code == sf::Keyboard::Key::Down) h = std::max(10, h - 2);
                if (keyEvent->code == sf::Keyboard::Key::Up) h = std::min(75, h + 2);
            }
        }

        window.clear(sf::Color(30, 30, 30));
        window.draw(title);
        window.draw(info);

        sf::Text txtW(font, "WIDTH:  < " + std::to_string(w) + " >", 35);
        sf::Text txtH(font, "HEIGHT: v " + std::to_string(h) + " ^", 35);

        sf::FloatRect bW = txtW.getLocalBounds();
        sf::FloatRect bH = txtH.getLocalBounds();

        txtW.setPosition({(window.getSize().x - bW.size.x) / 2.f, window.getSize().y * 0.4f});
        txtH.setPosition({(window.getSize().x - bH.size.x) / 2.f, window.getSize().y * 0.55f});

        window.draw(txtW);
        window.draw(txtH);
        window.display();
    }
    return {30, 30};
}
