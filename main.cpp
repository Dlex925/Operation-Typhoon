#include <SFML/Graphics.hpp>
#include "src/GameManager.h++"
#include <iostream>

int main() {
    try {
        sf::RenderWindow window(sf::VideoMode({2560, 1440}), "Operation Typhoon");

        std::string map = "assets/map1";
        Player p(3.0f,3.0f);
       // std::cout << map << "\n";
       // std::cout << player << "\n";
       // std::cout << raycast << "\n";

        GameManager game(window, map, p);
        std::cout << game << "\n";
        game.start();

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}