#include "MapEditor.h++"
#include <fstream>
#include <cmath>

MapEditor::MapEditor(sf::RenderWindow &win) : window(win), saveMsgText(font) {
    gridData.resize(gridHeight, std::vector<int>(gridWidth, 0));

    float uiX = static_cast<float>(window.getSize().x) * 0.76f;
    float uiY = 60.0f;

    setupButton(btnWMinus, uiX, uiY);
    setupButton(btnWPlus, uiX + 60, uiY);
    setupButton(btnHMinus, uiX, uiY + 50);
    setupButton(btnHPlus, uiX + 60, uiY + 50);

    resizeMap(24, 24);

    loadEditorAsset(font, "assets/0xProtoNerdFontPropo-Regular.ttf");
    if (font.getInfo().family != "") {
        fontLoaded = true;
        saveMsgText.setString("MAP SAVED!\nYOU CAN EXIT NOW (ESC)");
        saveMsgText.setCharacterSize(40);
        saveMsgText.setFillColor(sf::Color::Green);
        saveMsgText.setOutlineColor(sf::Color::Black);
        saveMsgText.setOutlineThickness(3.0f);
        saveMsgText.setStyle(sf::Text::Style::Bold);

        sf::FloatRect textRect = saveMsgText.getLocalBounds();
        saveMsgText.setOrigin({textRect.size.x / 2.0f, textRect.size.y / 2.0f});
        saveMsgText.setPosition({
            static_cast<float>(window.getSize().x) / 2.0f, static_cast<float>(window.getSize().y) / 2.0f
        });
    }

    // loadEditorAsset(tWall,   "assets/Wall.png"); I don't have a wall texture yet, and the cyan kinda grew on me so i might not replace it
    loadEditorAsset(tSR, "assets/EnemyShort_Idle.png");
    loadEditorAsset(tLR, "assets/EnemyLong_Idle.png");
    loadEditorAsset(tHD, "assets/EnemyLongHD_Idle.png");
    loadEditorAsset(tBoom, "assets/Boom_Idle.png");
    loadEditorAsset(tHealth, "assets/Medkit.png");
    loadEditorAsset(tAmmo, "assets/Ammo.png");
    loadEditorAsset(tMixed, "assets/Mixed.png");
    loadEditorAsset(tPlayer, "assets/Weapon1.png");
    loadEditorAsset(tPoints,"assets/Points.png");
}

void MapEditor::setupButton(sf::RectangleShape &btn, float x, float y) {
    btn.setSize({30.f, 30.f});
    btn.setFillColor(sf::Color(80, 80, 80));
    btn.setOutlineColor(sf::Color::White);
    btn.setOutlineThickness(1.f);
    btn.setPosition({x, y});
}

bool MapEditor::isClicked(const sf::RectangleShape &btn, const sf::Vector2i &mouse) {
    return btn.getGlobalBounds().contains({static_cast<float>(mouse.x), static_cast<float>(mouse.y)});
}

void MapEditor::resizeMap(int newW, int newH) {
    if (newW < 5) newW = 5;
    if (newW > 1023) newW = 1023;
    //Overkill, way overkill, especialy with the way butons are implemented, perhaps try an autoclicker if you want to reach this

    if (newH < 5) newH = 5;
    if (newH > 1023) newH = 1023;

    auto oldData = gridData;
    gridData.assign(newH, std::vector<int>(newW, 0));

    int copyH = std::min((int) oldData.size(), newH);
    int copyW = std::min((int) oldData[0].size(), newW);

    for (int y = 0; y < copyH; ++y) {
        for (int x = 0; x < copyW; ++x) {
            gridData[y][x] = oldData[y][x];
        }
    }

    gridWidth = newW;
    gridHeight = newH;

    float playAreaWidth = static_cast<float>(window.getSize().x) * 0.75f;
    float playAreaHeight = static_cast<float>(window.getSize().y);
    float sizeX = playAreaWidth / gridWidth;
    float sizeY = playAreaHeight / gridHeight;
    cellSize = std::min(sizeX, sizeY);
}

sf::Texture *MapEditor::getTextureForId(int id) {
    switch (id) {
        case 1: return &tWall;
        case 2: return &tSR;
        case 3: return &tLR;
        case 4: return &tHD;
        case 5: return &tBoom;
        case 6: return &tHealth;
        case 7: return &tAmmo;
        case 8: return &tMixed;
        case 9: return &tPoints;
        case 100: return &tPlayer;
        default: return nullptr;
    }
}

void MapEditor::saveMap(const std::string &filename) {
    std::ofstream out(filename);
    if (!out.is_open()) return;

    for (int i = 0; i < gridWidth + 2; ++i) out << '1';
    out << "\n";

    for (const auto &row: gridData) {
        out << '1';
        for (int cell: row) {
            out << cell;
        }
        out << '1';
        out << "\n";
    }

    for (int i = 0; i < gridWidth + 2; ++i) out << '1';
    out << "\n";

    double saveX = static_cast<double>(playerPos.x) + 1;
    double saveY = static_cast<double>(playerPos.y) + 1;

    out << saveX << " " << saveY << "\n";

    out.close();
    std::cout << "Map saved! Player pos: " << saveX << "," << saveY << "\n";
    showSaveMessage = true;
}

void MapEditor::handleInput() {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) window.close();

        if (const auto *keyEvent = event->getIf<sf::Event::KeyPressed>()) {
            if (keyEvent->code == sf::Keyboard::Key::Escape) window.close();
            if (keyEvent->code == sf::Keyboard::Key::S) saveMap("assets/map_created");

            if (keyEvent->code >= sf::Keyboard::Key::Num0 && keyEvent->code <= sf::Keyboard::Key::Num9)
                currentSelection = static_cast<int>(keyEvent->code) - static_cast<int>(sf::Keyboard::Key::Num0);
            if (keyEvent->code >= sf::Keyboard::Key::Numpad0 && keyEvent->code <= sf::Keyboard::Key::Numpad9)
                currentSelection = static_cast<int>(keyEvent->code) - static_cast<int>(sf::Keyboard::Key::Numpad0);

            if (keyEvent->code == sf::Keyboard::Key::P) {
                currentSelection = 100;
            }
        }

        if (const auto *mouseBtn = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mouseBtn->button == sf::Mouse::Button::Left) {
                sf::Vector2i mPos = mouseBtn->position;
                if (isClicked(btnWMinus, mPos)) resizeMap(gridWidth - 1, gridHeight);
                if (isClicked(btnWPlus, mPos)) resizeMap(gridWidth + 1, gridHeight);
                if (isClicked(btnHMinus, mPos)) resizeMap(gridWidth, gridHeight - 1);
                if (isClicked(btnHPlus, mPos)) resizeMap(gridWidth, gridHeight + 1);
            }
        }
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) || sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        if (mousePos.x < window.getSize().x * 0.75f) {
            int col = static_cast<int>(mousePos.x / cellSize);
            int row = static_cast<int>(mousePos.y / cellSize);

            if (col >= 0 && col < gridWidth && row >= 0 && row < gridHeight) {
                showSaveMessage = false;

                if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                    if (currentSelection == 100) {
                        playerPos = {col, row};
                    } else {
                        gridData[row][col] = currentSelection;
                    }
                } else {
                    gridData[row][col] = 0;
                }
            }
        }
    }
}

void MapEditor::render() {
    window.clear(sf::Color(30, 30, 30));

    sf::RectangleShape cellOutline({cellSize, cellSize});
    cellOutline.setFillColor(sf::Color::Transparent);
    cellOutline.setOutlineColor(sf::Color(60, 60, 60));
    cellOutline.setOutlineThickness(1.0f);

    for (int y = 0; y < gridHeight; ++y) {
        for (int x = 0; x < gridWidth; ++x) {
            float px = x * cellSize;
            float py = y * cellSize;
            if (px > window.getSize().x * 0.75f) continue;

            cellOutline.setPosition({px, py});
            window.draw(cellOutline);

            int id = gridData[y][x];
            if (id != 0) {
                sf::Texture *tex = getTextureForId(id);
                if (tex && tex->getSize().x > 0) {
                    sf::Sprite s(*tex);
                    float scaleX = cellSize / static_cast<float>(tex->getSize().x);
                    float scaleY = cellSize / static_cast<float>(tex->getSize().y);
                    s.setScale({scaleX, scaleY});
                    s.setPosition({px, py});
                    window.draw(s);
                } else {
                    sf::RectangleShape fallback({cellSize - 2, cellSize - 2});
                    fallback.setPosition({px + 1, py + 1});
                    if (id == 9) fallback.setFillColor(sf::Color::Cyan);
                    else if (id == 1) fallback.setFillColor(sf::Color::White);
                    else fallback.setFillColor(sf::Color::Magenta);
                    window.draw(fallback);
                }
            }
        }
    }

    if (tPlayer.getSize().x > 0) {
        sf::Sprite pSprite(tPlayer);
        float px = playerPos.x * cellSize;
        float py = playerPos.y * cellSize;
        pSprite.setColor(sf::Color(255, 255, 255, 200));
        float scaleX = cellSize / static_cast<float>(tPlayer.getSize().x);
        float scaleY = cellSize / static_cast<float>(tPlayer.getSize().y);
        pSprite.setScale({scaleX, scaleY});
        pSprite.setPosition({px, py});
        window.draw(pSprite);
    }

    window.draw(btnWMinus);
    window.draw(btnWPlus);
    window.draw(btnHMinus);
    window.draw(btnHPlus);

    if (fontLoaded) {
        float uiX = static_cast<float>(window.getSize().x) * 0.76f;

        sf::Text label(font, "Width: " + std::to_string(gridWidth), 18);
        label.setPosition({uiX, btnWMinus.getPosition().y - 25});
        window.draw(label);

        label.setString("Height: " + std::to_string(gridHeight));
        label.setPosition({uiX, btnHMinus.getPosition().y - 25});
        window.draw(label);

        sf::Text sym(font, "-", 24);
        sym.setPosition({btnWMinus.getPosition().x + 9, btnWMinus.getPosition().y - 4});
        window.draw(sym);
        sym.setPosition({btnHMinus.getPosition().x + 9, btnHMinus.getPosition().y - 4});
        window.draw(sym);
        sym.setString("+");
        sym.setPosition({btnWPlus.getPosition().x + 7, btnWPlus.getPosition().y - 4});
        window.draw(sym);
        sym.setPosition({btnHPlus.getPosition().x + 7, btnHPlus.getPosition().y - 4});
        window.draw(sym);

        std::string selName = (currentSelection == 100) ? "PLAYER START" : std::to_string(currentSelection);
        sf::Text text(font, "Selected: " + selName, 20);
        text.setPosition({uiX, 180.f});
        window.draw(text);

        sf::Texture *currTex = getTextureForId(currentSelection);
        if (currTex && currTex->getSize().x > 0) {
            sf::Sprite preview(*currTex);
            float pScale = 64.0f / std::max(currTex->getSize().x, currTex->getSize().y);
            preview.setScale({pScale, pScale});
            preview.setPosition({uiX, 220.f});
            window.draw(preview);
        }

        std::string list = "TOOLS:\n\n";
        list += "1: Wall\n";
        list += "2: SREnemy\n";
        list += "3: LREnemy\n";
        list += "4: HDEnemy\n";
        list += "5: Boom \n";
        list += "6: Health Pack\n";
        list += "7: Ammo Box\n";
        list += "8: Mixed Pack\n";
        list += "9: Points Pickup\n";
        list += "P: PLAYER START\n";
        list += "S: Save Map\nEsc: Back";

        sf::Text listText(font, list, 16);
        listText.setPosition({uiX, 300.f});
        window.draw(listText);

        if (showSaveMessage) {
            sf::RectangleShape bg({static_cast<float>(window.getSize().x) * 0.8f, 150.f});
            bg.setFillColor(sf::Color(0, 0, 0, 220));
            sf::Vector2f winCenter = {
                static_cast<float>(window.getSize().x) / 2.f, static_cast<float>(window.getSize().y) / 2.f
            };
            bg.setOrigin({bg.getSize().x / 2.f, bg.getSize().y / 2.f});
            bg.setPosition(winCenter);

            window.draw(bg);
            window.draw(saveMsgText);
        }
    }

    window.display();
}

void MapEditor::run() {
    while (window.isOpen()) {
        handleInput();
        render();
    }
}
