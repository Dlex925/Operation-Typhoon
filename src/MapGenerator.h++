#ifndef OOP_MAPGENERATOR_H
#define OOP_MAPGENERATOR_H

#include <vector>
#include <string>

class MapGenerator {
    int width, height;
    std::vector<std::vector<int>> map;
    int fillPercent;

    int getSurroundingWallCount(int gridX, int gridY);
    void smoothMap();

public:
    MapGenerator(int w, int h, int fillPct = 48);

    void generateLevel();
    void populateLevel();
    void saveToFile(const std::string& filename);
};

#endif //OOP_MAPGENERATOR_H