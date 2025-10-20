#include <SFML/Graphics.hpp>
#include "src/GameManager.h++"
#include <iostream>

int main() {
    try {
        sf::RenderWindow window(sf::VideoMode({2560, 1440}), "Operation Typhoon");

        Map map("assets/map1");
        Player player(3.0f, 3.0f);
        Raycast raycast(window, map, player);
       // std::cout << map << "\n";
       // std::cout << player << "\n";
       // std::cout << raycast << "\n";

        GameManager game(window, map, player, raycast);
        std::cout << game << "\n";
        game.start();

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}