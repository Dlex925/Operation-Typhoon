#ifndef OOP_MAP_H
#define OOP_MAP_H
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

class Map {
    unsigned long x_size, y_size;
    std::vector<std::vector<int>> map;

    public:
    explicit Map(const std::string& Filename);
    int isWall(double x , double y ) const ;
    Map(const Map& other)
    : x_size(other.x_size), y_size(other.y_size), map(other.map){};

    Map& operator=(const Map& other) {
        if (this != &other) {
            x_size = other.x_size;
            y_size = other.y_size;
            map = other.map;
        }
        return *this;
    }
    ~Map() {
        std::cout << "Map destroyed";
    }

    [[nodiscard]] unsigned long getWidth() const { return x_size; }
    [[nodiscard]] unsigned long getHeight() const { return y_size; }
    [[nodiscard]] int getCell(unsigned long x, unsigned long y) const {
        if (y >= map.size()) return 1; // treat out-of-bounds as wall
        if (x >= map[y].size()) return 1;
        return map[y][x];
    }

    friend std::ostream& operator<<(std::ostream& os, const Map& map);





};


#endif //OOP_MAP_H