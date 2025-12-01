#ifndef OOP_MAPEDITOR_H
#define OOP_MAPEDITOR_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <type_traits>

class MapEditor {
    sf::RenderWindow &window;
    int gridWidth = 24;
    int gridHeight = 24;
    float cellSize;

    std::vector<std::vector<int> > gridData;
    sf::Vector2i playerPos = {2, 2};

    int currentSelection = 1;
    sf::Font font;
    bool fontLoaded = false;

    sf::Texture tWall, tSR, tLR, tHD, tBoom, tHealth, tAmmo, tMixed, tPlayer, tPoints;

    sf::RectangleShape btnWMinus, btnWPlus, btnHMinus, btnHPlus;
    sf::Text saveMsgText;
    bool showSaveMessage = false;

    void setupButton(sf::RectangleShape &btn, float x, float y);

    bool isClicked(const sf::RectangleShape &btn, const sf::Vector2i &mousePos);

    void resizeMap(int newW, int newH);

    template<typename T>
    void loadEditorAsset(T &asset, const std::string &path) {
        bool success = false;
        if constexpr (std::is_same_v<T, sf::Font>) success = asset.openFromFile(path);
        else success = asset.loadFromFile(path);
        if (!success) std::cerr << "[MapEditor] Warning: Failed to load " << path << "\n";
    }

    void saveMap(const std::string &filename);

    void handleInput();

    void render();

    sf::Texture *getTextureForId(int id);

public:
    explicit MapEditor(sf::RenderWindow &win);

    void run();
};

#endif //OOP_MAPEDITOR_H
