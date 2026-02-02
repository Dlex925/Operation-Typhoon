//
// Created by dlex on 06.11.2025.
//

#ifndef OOP_MENU_H
#define OOP_MENU_H

#include <SFML/Graphics.hpp>


class Menu {
public:
    static Menu &getInstance();

    sf::VideoMode selectResolution();

    bool wasFullscreenChosen() const { return fullscreenChosen_; }

    int selectGameMode(sf::RenderWindow &window);

    std::pair<int, int> selectGenerationSize(sf::RenderWindow &window);

    std::string askName(sf::RenderWindow &window, int score);

    void saveHighscore(const std::string &name, int score);

    void showHighscores(sf::RenderWindow &window);

    bool isGodModeActive() const { return godModeActive_; }

private:
    Menu() = default;

    bool godModeActive_ = false;

    ~Menu() = default;

    Menu(const Menu &) = delete;

    Menu &operator=(const Menu &) = delete;

    Menu(Menu &&) = delete;

    Menu &operator=(Menu &&) = delete;

    bool fullscreenChosen_ = false;
};


#endif //OOP_MENU_H
