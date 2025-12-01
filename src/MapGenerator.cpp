#include "MapGenerator.h++"
#include <random>
#include <fstream>
#include <iostream>
#include <ctime>

struct GenPoint {
    int x, y; 
};

MapGenerator::MapGenerator(int w, int h, int fillPct) 
    : width(w), height(h), fillPercent(fillPct) {
    map.resize(height, std::vector<int>(width, 0));
}

void MapGenerator::generateLevel() {
    std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr)));
    std::uniform_int_distribution<int> dist(0, 100);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (x == 0 || x == width - 1 || y == 0 || y == height - 1) {
                map[y][x] = 1; // Margini
            } else {
                map[y][x] = (dist(rng) < fillPercent) ? 1 : 0;
            }
        }
    }
    for (int i = 0; i < 5; ++i) {
        smoothMap();
    }
}

int MapGenerator::getSurroundingWallCount(int gridX, int gridY) {
    int wallCount = 0;
    for (int neighborY = gridY - 1; neighborY <= gridY + 1; ++neighborY) {
        for (int neighborX = gridX - 1; neighborX <= gridX + 1; ++neighborX) {
            if (neighborX >= 0 && neighborX < width && neighborY >= 0 && neighborY < height) {
                if (neighborX != gridX || neighborY != gridY) {
                    if (map[neighborY][neighborX] == 1) {
                        wallCount++;
                    }
                }
            } else {
                wallCount++;
            }
        }
    }
    return wallCount;
}

void MapGenerator::smoothMap() {
    std::vector<std::vector<int>> newMap = map;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int neighborWallTiles = getSurroundingWallCount(x, y);

            if (neighborWallTiles > 4)
                newMap[y][x] = 1;
            else if (neighborWallTiles < 4)
                newMap[y][x] = 0;
        }
    }
    map = newMap;
}

void MapGenerator::populateLevel() {
    std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr)));
    
    std::vector<GenPoint> freeSpots;
    for(int y=1; y<height-1; ++y) {
        for(int x=1; x<width-1; ++x) {
            if(map[y][x] == 0) freeSpots.push_back({x, y});
        }
    }

    if(freeSpots.empty()) return;

    auto popRandomSpot = [&](int idToPlace) {
        if(freeSpots.empty()) return;
        std::uniform_int_distribution<size_t> d(0, freeSpots.size() - 1);
        size_t index = d(rng);
        GenPoint p = freeSpots[index];
        
        map[p.y][p.x] = idToPlace;
        
        freeSpots[index] = freeSpots.back();
        freeSpots.pop_back();
    };
    int area = width * height;
    for(int i=0; i<std::max(area / 100, 1); ++i) popRandomSpot(2); // SR Enemy
    for(int i=0; i<std::max(area / 350, 1); ++i) popRandomSpot(3); // LR Enemy
    for(int i=0; i<std::max(area / 600, 1); ++i) popRandomSpot(4); // HD Enemy
    for(int i=0; i<std::max(area / 1250, 0); ++i) popRandomSpot(5); // Boom
    for(int i=0; i<std::max(area / 80, 1); ++i) popRandomSpot(6); // HP
    for(int i=0; i<std::max(area / 80, 1); ++i) popRandomSpot(7); // Ammo
    for(int i=0; i<std::max(area / 120, 1); ++i) popRandomSpot(8); // Mixed
    for(int i=0; i<std::max(area / 25, 1); ++i) popRandomSpot(9); // Points
}

void MapGenerator::saveToFile(const std::string& filename) {
    std::ofstream out(filename);
    if (!out.is_open()) return;

    double pX = 2.5, pY = 2.5;
    bool found = false;
    for(int y=2; y<height-2 && !found; ++y) {
        for(int x=2; x<width-2 && !found; ++x) {
            if(map[y][x] == 0) {
                pX = x + 0.5;
                pY = y + 0.5;
                found = true;
            }
        }
    }

    for (const auto& row : map) {
        for (int cell : row) {
            out << cell;
        }
        out << "\n";
    }

    out << pX << " " << pY << "\n";


    out.close();
    std::cout << "[MapGenerator] Level saved to " << filename << "\n";
}