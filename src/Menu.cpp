//
// Created by dlex on 06.11.2025.
//

#include "Menu.h++"
#include <algorithm>
#include <string>
#include <vector>
#include <memory>
#include <SFML/Audio.hpp>

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
        instructions->setFillColor(sf::Color(200, 200, 200));
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
                    sleep(1);
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
