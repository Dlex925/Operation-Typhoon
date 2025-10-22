#include "GameManager.h++"
#include <iostream>
#include <SFML/Audio.hpp>


GameManager::GameManager(sf::RenderWindow& win, std::string harta, Player& p)
    : window(win), map(harta), player(p), raycast(window,map,player) {}

void GameManager::start() const{
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
        Engine();
    }
}

void GameManager::handleInput(float deltaTime) const{

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
        player.shoot();
    }
}

void GameManager::Engine()const {
    window.clear(sf::Color(0, 0, 0));
    raycast.render();
    player.drawWeapon(window);
    window.display();

}