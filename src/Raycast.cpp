#include "Raycast.h++"
#include <cmath>

Raycast::Raycast(sf::RenderWindow& win, Map& m, Player& p )
    : window(&win), map(&m), player(&p) {}
void Raycast::render() const{
     int w = static_cast<int>(window->getSize().x);
     int h = static_cast<int>(window->getSize().y);
     depthBuffer.resize(w);

     for (int x = 0; x < w; x++) {
         double cameraX = 2 * x / static_cast<double>(w) - 1;
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
        depthBuffer[x] = perpWallDist;

        sf::Color color = sf::Color(255, 255, 255);
        sf::Color color2 = sf::Color(0, 255, 255);


        sf::Vertex line[2];
        line[0].position = sf::Vector2f(static_cast<float>(x), static_cast<float>(drawStart));
        line[0].color = color;
        line[1].position = sf::Vector2f(static_cast<float>(x), static_cast<float>(drawEnd));
        line[1].color = color2;


        window->draw(line, 2, sf::PrimitiveType::Lines);
     }
}

void Raycast::renderEnemy(const Enemy &enemy) const {
    static sf::Texture enemyTex1("assets/Enemy1.png");
    static sf::Texture enemyTex2("assets/Enemy2.png");
    int w = static_cast<int>(window->getSize().x);
    int h = static_cast<int>(window->getSize().y);

    double spriteX = enemy.getWorldX() - player->getX();
    double spriteY = enemy.getWorldY() - player->getY();

    double invDet = 1.0 / (player->getPlaneX() * player->getDirY() - player->getDirX() * player->getPlaneY());

    double transformX = invDet * (player->getDirY() * spriteX - player->getDirX() * spriteY);
    double transformY = invDet * (-player->getPlaneY() * spriteX + player->getPlaneX() * spriteY);

    if (transformY <= 0.0001) {
        return;
    }

    int spriteScreenX = static_cast<int>((w / 2.0) * (1 + transformX / transformY));

    int spriteHeight = std::abs(static_cast<int>(h / transformY * enemy.getBaseHeight()));
    int drawStartY = std::max(-spriteHeight / 2 + h / 2, 0);
    int drawEndY = std::min(spriteHeight / 2 + h / 2, h - 1);

    int spriteWidth = spriteHeight;
    int drawStartX = std::max(-spriteWidth / 2 + spriteScreenX, 0);
    int drawEndX = std::min(spriteWidth / 2 + spriteScreenX, w - 1);

    if (drawStartX >= w || drawEndX < 0) return;

    const bool hasTex1 = enemyTex1.getSize().x > 0 && enemyTex1.getSize().y > 0;
    const bool hasTex2 = enemyTex2.getSize().x > 0 && enemyTex2.getSize().y > 0;

    const sf::Texture *tex = nullptr;
    if (enemy.isDead()) {
        if (hasTex2) tex = &enemyTex2;
        else if (hasTex1) tex = &enemyTex1;
    } else {
        if (hasTex1) tex = &enemyTex1;
        else if (hasTex2) tex = &enemyTex2;
    }

    unsigned texW = tex ? tex->getSize().x : 0;
    unsigned texH = tex ? tex->getSize().y : 0;

    int unclippedStartY = -spriteHeight / 2 + h / 2;
    //int unclippedEndY   =  spriteHeight / 2 + h / 2;

    int visibleStart = w;
    int visibleEnd = -1;
    for (int stripe = drawStartX; stripe <= drawEndX; ++stripe) {
        if (stripe < 0 || stripe >= static_cast<int>(depthBuffer.size())) continue;
        if (transformY < depthBuffer[stripe]) {
            int screenLeft = -spriteWidth / 2 + spriteScreenX;
            double rel = static_cast<double>(stripe - screenLeft) / static_cast<double>(spriteWidth);
            if (rel < 0.0 || rel > 1.0) continue;
            int texX = static_cast<int>(rel * static_cast<double>(texW));
            if (texX < 0) texX = 0;
            if (texX >= static_cast<int>(texW)) texX = static_cast<int>(texW) - 1;

            int visTop = drawStartY;
            int visBot = drawEndY;
            double topFrac = static_cast<double>(visTop - unclippedStartY) / static_cast<double>(spriteHeight);
            double botFrac = static_cast<double>(visBot - unclippedStartY + 1) / static_cast<double>(spriteHeight);
            if (topFrac < 0.0) topFrac = 0.0;
            if (topFrac > 1.0) topFrac = 1.0;
            if (botFrac < 0.0) botFrac = 0.0;
            if (botFrac > 1.0) botFrac = 1.0;
            int texTop = static_cast<int>(topFrac * static_cast<double>(texH));
            int texBottom = static_cast<int>(botFrac * static_cast<double>(texH));
            int texHeightSub = texBottom - texTop;
            if (texHeightSub <= 0) texHeightSub = 1;

            sf::RectangleShape col(sf::Vector2f(1.f, static_cast<float>(drawEndY - drawStartY + 1)));
            col.setPosition(sf::Vector2f(static_cast<float>(stripe), static_cast<float>(drawStartY)));
            col.setTexture(tex);
            col.setTextureRect(sf::IntRect(sf::Vector2i(texX, texTop), sf::Vector2i(1, texHeightSub)));
            window->draw(col);

            if (stripe < visibleStart) visibleStart = stripe;
            if (stripe > visibleEnd) visibleEnd = stripe;
        }
    }

    if (visibleEnd >= visibleStart) {
        int visWidth = visibleEnd - visibleStart + 1;
        float barMaxWidth = static_cast<float>(std::max(24, spriteWidth / 2));
        float finalBarWidth = std::min(barMaxWidth, static_cast<float>(visWidth));
        float hpRatio = 1.f;
        if (enemy.getMaxHp() > 0) {
            hpRatio = static_cast<float>(enemy.getHp()) / static_cast<float>(enemy.getMaxHp());
            if (hpRatio < 0.f) hpRatio = 0.f;
            if (hpRatio > 1.f) hpRatio = 1.f;
        }
        float barHeight = 4.f;
        float centerX = (static_cast<float>(visibleStart + visibleEnd)) * 0.5f;
        float barX = centerX - finalBarWidth * 0.5f;
        float barY = static_cast<float>(std::max(0, drawStartY - 8));

        sf::RectangleShape back(sf::Vector2f(finalBarWidth, barHeight));
        back.setPosition(sf::Vector2f(barX, barY));
        back.setFillColor(sf::Color(60, 0, 0));

        sf::RectangleShape fill(sf::Vector2f(finalBarWidth * hpRatio, barHeight));
        fill.setPosition(sf::Vector2f(barX, barY));
        fill.setFillColor(sf::Color(0, 220, 0));

        window->draw(back);
        window->draw(fill);
    }
}

std::ostream& operator<<(std::ostream& os, const Raycast& raycast) {
    os << "Raycast(window: " << (raycast.window ? "active" : "null")
          << ", map: " << (raycast.map ? "loaded" : "null")
          << ", player: " << (raycast.player ? "active" : "null") << ")";
    return os;
}
