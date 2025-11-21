#include "Map.h++"
//#include "Exceptions.h++"

#include <cmath>
#include <iostream>
#include "Exceptions.h++"

Map::Map(const std::string& FileName) {
    std::ifstream fin(FileName);
    if (!fin.is_open()) {
        throw MapException("Cannot open map file: " + FileName);
    }
    std::string line;
    while (std::getline(fin, line)) {
        std::vector<int> row;

        for (char c : line) {
            if (c >= '0' && c <= '9') {
                row.push_back(c - '0');
            } /*else if (!std::isspace(static_cast<unsigned char>(c))) {
                throw MapException("Invalid character in map file: '" + std::string(1, c) + "'");
            }*/
        }
        if (!row.empty()) {
            map.push_back(row);
        }
    }
    if (map.empty()) {
        throw MapException("Empty map");
    }
    y_size = map.size();
    x_size = map[0].size();
    std::cout << "Map loaded: " << x_size << "x" << y_size << std::endl;
    fin.close();

}

int Map::isWall(double x, double y) const{
    int ix = static_cast<int>(std::trunc(x));
    int iy = (static_cast<int>(std::trunc(y)));

    if (static_cast<unsigned long>(iy) >= map.size() ||
        static_cast<unsigned long>(ix) >= map[iy].size()) {
        return 1;
    }

    // Treat only tile '1' as wall; others (0=empty, 2/3/4=enemies) are walkable
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



