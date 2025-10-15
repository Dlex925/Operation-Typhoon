#include <SFML/Graphics.hpp>
#include <iostream>
#include <optional>
#include "SFML/Audio/Music.hpp"
#include "src/Map.h++"
#include "src/Player.h++"
#include "src/Raycast.h++"

int main() {
    try {
        sf::RenderWindow window(sf::VideoMode({2560, 1440}), "Operation Typhoon");
        window.setFramerateLimit(144);
        window.setMouseCursorVisible(false);

        Map map("assets/map1");
        sf::Music music;
        if (!music.openFromFile("assets/ezoteric.ogg"))
            return -1; // error
        // De adaugat check pentru music
        music.play();
        Player player(3.0f, 3.0f);
        Raycast raycast(window, map, player);

        sf::Clock clock;
        sf::Mouse::setPosition({400, 300}, window);

        while (window.isOpen()) {
            float deltaTime = clock.restart().asSeconds();

            while (std::optional<sf::Event> event = window.pollEvent()) {
                if (event && event->is<sf::Event::Closed>()) window.close();
            }

            // Mouse rotation
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            if (mousePos.x != 400) {
                player.RotateCamera(static_cast<float>(400 - mousePos.x));
                sf::Mouse::setPosition({400, 300}, window);
            }

            // Keyboard movement
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) player.MoveForward(deltaTime, map);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) player.MoveBackward(deltaTime, map);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) player.MoveLeft(deltaTime, map);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) player.MoveRight(deltaTime, map);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) window.close();

            // Render
            window.clear(sf::Color(0, 0, 0));

            raycast.render();

            window.display();
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}