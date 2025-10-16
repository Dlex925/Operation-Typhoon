#ifndef OOP_MAP_H
#define OOP_MAP_H
#include <string>
#include <vector>
#include <fstream>

class Map {
    unsigned long x_size, y_size;
    std::vector<std::vector<int>> map;

    public:
    explicit Map(const std::string& Filename);
    int isWall(double x , double y ) ;
   [[nodiscard]] unsigned long  getTileSizeX()const {return x_size;}
   [[nodiscard]] unsigned long  getTileSizeY()const {return y_size;}





};


#endif //OOP_MAP_H