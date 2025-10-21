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
  // [[nodiscard]] unsigned long  getTileSizeX()const {return x_size;}
  // [[nodiscard]] unsigned long  getTileSizeY()const {return y_size;}
    friend std::ostream& operator<<(std::ostream& os, const Map& map);





};


#endif //OOP_MAP_H