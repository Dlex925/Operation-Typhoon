#include "Raycast.h++"
#include <cmath>
#include <unordered_map>
#include <string>
#include "Enemy.h++"

Raycast::Raycast(sf::RenderWindow& win, Map& m, Player& p )
    : window{&win}, map{&m}, player{&p} {
}

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
    struct Textures {
        sf::Texture idle, attack, dead, attack2;
        bool loaded = false;
    };
    static std::unordered_map<std::string, Textures> cache;
    std::string type = enemy.typeName();
    auto it = cache.find(type);
    if (it == cache.end()) {
        Textures t;
        t.idle = sf::Texture("assets/" + type + "_Idle.png");
        t.attack = sf::Texture("assets/" + type + "_Attack.png");
        std::string Attack2 = "assets/" + type + "_Attack2.png";
        if (std::filesystem::exists(Attack2)) {
            t.attack2 = sf::Texture("assets/" + type + "_Attack2.png");
        } else {
            t.attack2 = sf::Texture("assets/" + type + "_Attack.png");
        }
        t.dead = sf::Texture("assets/" + type + "_Dead.png");
        t.loaded = true;
        it = cache.emplace(type, std::move(t)).first;
    }

    bool isDead = enemy.isDead();
    bool isAttacking = false;
    bool In_short_range = false;
    if (!isDead) {
        double dx = enemy.getWorldX() - player->getX();
        double dy = enemy.getWorldY() - player->getY();
        double dist2 = dx * dx + dy * dy;
        double rng = enemy.attackRange();
        isAttacking = dist2 <= rng * rng;
        In_short_range = dist2 <= rng * rng * 0.2;
    }

    const sf::Texture *tex = nullptr;
    if (isDead) {
        if (it->second.dead.getSize().x > 0) tex = &it->second.dead;
    } else if (isAttacking) {
        if (it->second.attack.getSize().x > 0 && In_short_range)tex = &it->second.attack2;
    } else {
        if (it->second.idle.getSize().x > 0) tex = &it->second.idle;
    }

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

    unsigned texW = tex ? tex->getSize().x : 0;
    unsigned texH = tex ? tex->getSize().y : 0;

    int unclippedStartY = -spriteHeight / 2 + h / 2;

    int visibleStart = w;
    int visibleEnd = -1;
    for (int stripe = drawStartX; stripe <= drawEndX; ++stripe) {
        if (stripe < 0 || stripe >= static_cast<int>(depthBuffer.size())) continue;
        if (transformY < depthBuffer[stripe]) {
            int screenLeft = -spriteWidth / 2 + spriteScreenX;
            double rel = static_cast<double>(stripe - screenLeft) / static_cast<double>(spriteWidth);
            if (rel < 0.0 || rel > 1.0) continue;
            int texX = texW > 0 ? static_cast<int>(rel * static_cast<double>(texW)) : 0;
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
            int texTop = texH > 0 ? static_cast<int>(topFrac * static_cast<double>(texH)) : 0;
            int texBottom = texH > 0 ? static_cast<int>(botFrac * static_cast<double>(texH)) : 0;
            int texHeightSub = texBottom - texTop;
            if (texHeightSub <= 0) texHeightSub = 1;

            sf::RectangleShape col(sf::Vector2f(1.f, static_cast<float>(drawEndY - drawStartY + 1)));
            col.setPosition(sf::Vector2f(static_cast<float>(stripe), static_cast<float>(drawStartY)));
            col.setTexture(tex);
            if (tex) {
                col.setTextureRect(sf::IntRect(sf::Vector2i(texX, texTop), sf::Vector2i(1, texHeightSub)));
            }
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

void Raycast::renderPickup(const PickupBase &pickup) const {
    static std::unordered_map<std::string, sf::Texture> cache;

    std::string type = pickup.getTextureType();
    auto it = cache.find(type);

    if (it == cache.end()) {
        sf::Texture t;
        std::string path = "assets/" + type + ".png";
        if (std::filesystem::exists(path)) {
            if (!t.loadFromFile(path)) {
                std::cerr << "Missing " << path << "\n";
            }
        }

        it = cache.emplace(type, std::move(t)).first;
    }

    const sf::Texture *tex = &it->second;

    if (tex->getSize().x == 0) return;

    int w = static_cast<int>(window->getSize().x);
    int h = static_cast<int>(window->getSize().y);

    double spriteX = pickup.getX() - player->getX();
    double spriteY = pickup.getY() - player->getY();

    double invDet = 1.0 / (player->getPlaneX() * player->getDirY() - player->getDirX() * player->getPlaneY());

    double transformX = invDet * (player->getDirY() * spriteX - player->getDirX() * spriteY);
    double transformY = invDet * (-player->getPlaneY() * spriteX + player->getPlaneX() * spriteY);

    if (transformY <= 0.0001) {
        return;
            /*
            Checks that the object isnt behind the player and also prevents division by 0 errors.
            There is no need to check transformX as it can be negative or positive - right relative to the center of the camera or left
             */
    }

    int spriteScreenX = static_cast<int>((w / 2.0) * (1 + transformX / transformY));


    int vMoveScreen = static_cast<int>(500.0 / transformY);
    int spriteHeight = std::abs(static_cast<int>(h / transformY)) / 2;

    int drawStartY = -spriteHeight / 2 + h / 2 + vMoveScreen;
    if (drawStartY < 0) drawStartY = 0;
    int drawEndY = spriteHeight / 2 + h / 2 + vMoveScreen;
    if (drawEndY >= h) drawEndY = h - 1;

    int spriteWidth = std::abs(static_cast<int>(h / transformY)) / 2;
    int drawStartX = -spriteWidth / 2 + spriteScreenX;
    if (drawStartX < 0) drawStartX = 0;
    int drawEndX = spriteWidth / 2 + spriteScreenX;
    if (drawEndX >= w) drawEndX = w - 1;

    if (drawStartX >= w || drawEndX < 0) return;

    unsigned texW = tex->getSize().x;
    unsigned texH = tex->getSize().y;

    int unclippedStartY = -spriteHeight / 2 + h / 2 + vMoveScreen;

    for (int stripe = drawStartX; stripe <= drawEndX; ++stripe) {
        if (stripe < 0 || stripe >= static_cast<int>(depthBuffer.size())) continue;

        if (transformY < depthBuffer[stripe]) {
            int screenLeft = -spriteWidth / 2 + spriteScreenX;
            int texX = static_cast<int>(256 * (stripe - screenLeft) * texW / spriteWidth) / 256;

            if (texX < 0) texX = 0;
            if (texX >= static_cast<int>(texW)) texX = static_cast<int>(texW) - 1;

            int visTop = drawStartY;
            int visBot = drawEndY;

            double topFrac = static_cast<double>(visTop - unclippedStartY) / static_cast<double>(spriteHeight);
            double botFrac = static_cast<double>(visBot - unclippedStartY + 1) / static_cast<double>(spriteHeight);

            if (topFrac < 0.0) { topFrac = 0.0; }
            if (topFrac > 1.0) { topFrac = 1.0; }
            if (botFrac < 0.0) { botFrac = 0.0; }
            if (botFrac > 1.0) { botFrac = 1.0; }

            int texTop = static_cast<int>(topFrac * static_cast<double>(texH));
            int texBottom = static_cast<int>(botFrac * static_cast<double>(texH));
            int texHeightSub = texBottom - texTop;

            if (texHeightSub <= 0) texHeightSub = 1;

            sf::RectangleShape col(sf::Vector2f(1.f, static_cast<float>(drawEndY - drawStartY + 1)));
            col.setPosition(sf::Vector2f(static_cast<float>(stripe), static_cast<float>(drawStartY)));
            col.setTexture(tex);
            col.setTextureRect(sf::IntRect({texX, texTop}, {1, texHeightSub}));
            window->draw(col);
        }
    }
}
