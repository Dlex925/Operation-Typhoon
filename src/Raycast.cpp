#include "Raycast.h++"

Raycast::Raycast(sf::RenderWindow& win, Map& m, Player& p )
    : window(&win), map(&m), player(&p) {}
void Raycast::render() {
    int w = window->getSize().x;
    int h = window->getSize().y;

    for (int x = 0; x < w; x++) {
        float cameraX = 2 * x / float(w) - 1;
        float rayDirX = player->getDirX() + player->getPlaneX() * cameraX;
        float rayDirY = player->getDirY() + player->getPlaneY() * cameraX;

        int mapX = static_cast<int>(player->getX());
        int mapY = static_cast<int>(player->getY());

        float deltaDistX = (rayDirX == 0) ? 1e30 : std::abs(1 / rayDirX);
        float deltaDistY = (rayDirY == 0) ? 1e30 : std::abs(1 / rayDirY);

        float sideDistX, sideDistY;
        int stepX, stepY;

        if (rayDirX < 0) {
            stepX = -1;
            sideDistX = (player->getX() - mapX) * deltaDistX;
        } else {
            stepX = 1;
            sideDistX = (mapX + 1.0f - player->getX()) * deltaDistX;
        }
        if (rayDirY < 0) {
            stepY = -1;
            sideDistY = (player->getY() - mapY) * deltaDistY;
        } else {
            stepY = 1;
            sideDistY = (mapY + 1.0f - player->getY()) * deltaDistY;
        }

        int hit = 0, side;
        while (hit == 0) {
            if (sideDistX < sideDistY) {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            } else {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }
            if (map->isWall(static_cast<float>(mapX), static_cast<float>(mapY))) hit = 1;
        }

        float perpWallDist = (side == 0) ? (sideDistX - deltaDistX) : (sideDistY - deltaDistY);
        int lineHeight = static_cast<int>(h / perpWallDist);
        int drawStart = std::max(-lineHeight / 2 + h / 2, 0);
        int drawEnd = std::min(lineHeight / 2 + h / 2, h - 1);

       sf::Color color =  sf::Color(255, 255, 255);
        sf::Color color2 =  sf::Color(0, 255, 255);
            sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(float(x), float(drawStart)), color),
            sf::Vertex(sf::Vector2f(float(x), float(drawEnd)), color2)
        };


        window->draw(line, 2, sf::PrimitiveType::Lines);
    }
}
