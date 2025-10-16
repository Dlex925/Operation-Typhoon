#include "Raycast.h++"

Raycast::Raycast(sf::RenderWindow& win, Map& m, Player& p )
    : window(&win), map(&m), player(&p) {}
void Raycast::render() {
     int w = static_cast<int>(window->getSize().x);
     int h = static_cast<int>(window->getSize().y);

    for (int x = 0; x < w; x++) {
        double cameraX = 2 * x / static_cast<double
        >(w) - 1;
        double rayDirX = player->getDirX() + player->getPlaneX() * cameraX;
        double rayDirY = player->getDirY() + player->getPlaneY() * cameraX;

        int mapX = static_cast<int>(player->getX());
        int mapY = static_cast<int>(player->getY());

        double deltaDistX = (rayDirX == 0) ? 1e30 : std::abs(1 / rayDirX);
        double deltaDistY = (rayDirY == 0) ? 1e30 : std::abs(1 / rayDirY);

        double sideDistX, sideDistY;
        int stepX, stepY;

        if (rayDirX < 0) {
            stepX = -1;
            sideDistX = (player->getX() - mapX) * deltaDistX;
        } else {
            stepX = 1;
            sideDistX = (static_cast<double>(mapX) + 1.0f - player->getX()) * deltaDistX;
        }
        if (rayDirY < 0) {
            stepY = -1;
            sideDistY = (player->getY() - mapY) * deltaDistY;
        } else {
            stepY = 1;
            sideDistY = (static_cast<double>(mapY) + 1.0f - player->getY()) * deltaDistY;
        }

        int hit = 0, side = 0 ;
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
            if (map->isWall(static_cast<double>(mapX), static_cast<double>(mapY))) hit = 1;
        }

        double perpWallDist = (side == 0) ? (sideDistX - deltaDistX) : (sideDistY - deltaDistY);
        int lineHeight = static_cast<int>(h / perpWallDist);
        int drawStart = std::max(-lineHeight / 2 + h / 2, 0);
        int drawEnd = std::min(lineHeight / 2 + h / 2, h - 1);

       sf::Color color =  sf::Color(255, 255, 255);
        sf::Color color2 =  sf::Color(0, 255, 255);
            sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(static_cast<float>(x), static_cast<float>(drawStart)), color),
            sf::Vertex(sf::Vector2f(static_cast<float>(x), static_cast<float>(drawEnd)), color2)
        };


        window->draw(line, 2, sf::PrimitiveType::Lines);
    }
}
