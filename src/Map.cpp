#include "Map.h++"
#include <cmath>
#include <iostream>
#include "Exceptions.h++"
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

Map::Map(const std::string& FileName, double& outPlayerX, double& outPlayerY) {
    std::ifstream fin(FileName);
    if (!fin.is_open()) {
        throw MapException("Cannot open map file: " + FileName);
    }

    std::vector<std::string> lines;
    std::string line;

    while (std::getline(fin, line)) {
        if (!line.empty()) lines.push_back(line);
    }
    fin.close();

    if (lines.size() < 2) throw MapException("Map file too short");

    std::string coordsLine = lines.back();
    lines.pop_back();

    std::stringstream ss(coordsLine);
    std::cout <<outPlayerX << ", " << outPlayerY << "\n";
    if (!(ss >> outPlayerX >> outPlayerY)) {
        std::cout <<outPlayerX << ", " << outPlayerY << "\n";

        outPlayerX = 3; outPlayerY = 3;
        std::cerr << "[Map] Warning: Failed to read player coords\n";
    }
    std::cout <<outPlayerX << ", " << outPlayerY << "\n";

    unsigned long max_width = 0;
    for (const auto& mapRowStr : lines) {
        std::vector<int> row;
        for (char c : mapRowStr) {
            if (c >= '0' && c <= '9') {
                row.push_back(c - '0');
            }
        }
        if (!row.empty()) {
            if (row.size() > max_width) max_width = row.size();
            map.push_back(row);
        }
    }

    for (auto& row : map) {
        while (row.size() < max_width) row.push_back(1);
    }

    if (map.empty()) throw MapException("Empty map grid");
    y_size = map.size();
    x_size = max_width;

    std::cout << "Map Loaded: " << x_size << "x" << y_size << "\n";
    std::cout << " > Player Spawn Output: " << outPlayerX << ", " << outPlayerY << "\n";
}

int Map::isWall(double x, double y) const{
    int ix = static_cast<int>(std::trunc(x));
    int iy = (static_cast<int>(std::trunc(y)));

    if (static_cast<unsigned long>(iy) >= map.size() ||
        static_cast<unsigned long>(ix) >= map[iy].size()) {
        return 1;
    }

    return map[iy][ix] == 1;
}

std::ostream& operator<<(std::ostream& os, const Map& map) {
    os << "Map " << map.x_size << "x" << map.y_size << ":\n";

    os << "   █";
    for (unsigned long x = 0; x < map.x_size; ++x) os << "█";
    os << "█\n";

    for (unsigned long y = 0; y < map.y_size; ++y) {
        os << "   █";
        for (unsigned long x = 0; x < map.x_size; ++x) {
            if (map.map[y][x] == 1) {
                os << "█";
            } else {
                os << " ";
            }
        }
        os << "█\n";
    }

    os << "   █";
    for (unsigned long x = 0; x < map.x_size; ++x) os << "█";
    os << "█\n";

    return os;
}



