#include "Map.h++"
#include <iostream>
Map::Map(const std::string& FileName) {
    std::ifstream fin(FileName);
    if (!fin.is_open()) {
        throw std::runtime_error("Map::Map(): cannot open file");
    }
    std::string line;
    while (std::getline(fin, line)) {
        std::vector<int> row;

        for (char c : line) {
            if (c >= '0' && c <= '9') {
                row.push_back(c - '0');

            }
        }
            if (!row.empty()) {map.push_back(row);}

    }
    if (map.empty()) {
        throw std::runtime_error("Map::Map(): empty map");
    }
    y_size = map.size();
    x_size = map[0].size();
    std::cout << "Map loaded: " << x_size << "x" << y_size << std::endl;
    fin.close();

}

int Map::isWall(double x, double y) {
    int ix = static_cast<int>(x);
    int iy = static_cast<int>(y);

    if (static_cast<unsigned long>(iy) >= map.size() ||
    static_cast<unsigned long>(ix) >= map[iy].size()) {
        return 1;
    }

    return map[iy][ix] != 0;
}

