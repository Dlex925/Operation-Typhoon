#include <SFML/Graphics.hpp>
#include "src/GameManager.h++"
#include "src/Menu.h++"
#include <iostream>

#include "src/Exceptions.h++"
#include "src/MapEditor.h++"

int main() {
    try {
        auto &menu = Menu::getInstance();
        auto selectedMode = menu.selectResolution();

        std::uint32_t style = sf::Style::Default;
        if (menu.wasFullscreenChosen()) {
            auto desktop = sf::VideoMode::getDesktopMode();
            selectedMode = sf::VideoMode({desktop.size.x, desktop.size.y});
            style = sf::Style::None;
        }
        sf::RenderWindow window(selectedMode, "Operation Typhoon", style);
        if (menu.wasFullscreenChosen()) {
            window.setPosition({0, 0});
        }

        int mode = menu.selectGameMode(window);

        if (mode == 1) {
            std::string map = "assets/map1";
            Player p(3.0f, 3.0f);
            GameManager game(window, map, p);
            // std::cout << game << "\n";
            game.start();
        }
        else if (mode == 2) {
            MapEditor editor(window);
            editor.run();
        }
    } catch (const AssetLoadException &e) {
        std::cerr << e.what() << "\n";
        return -1;
    } catch (const MapException &e) {
        std::cerr << e.what() << "\n";
        return -1;
    } catch (const UnknownEnemyTypeException &e) {
        std::cerr << e.what() << "\n";
        return -1;
    } catch (const GameException &e) {
        std::cerr << e.what() << "\n";
        return -1;
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << "\n";
        return -1;
    } catch (...) { std::cerr << "Unknown Error\n"; }

    return 0;
}
