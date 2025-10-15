#ifndef OOP_MAP_H
#define OOP_MAP_H
#include <fstream>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>


class Map {
    int x_size, y_size;
    std::vector<std::vector<int>> map;

    public:
    Map(const std::string& Filename);
    bool isWall(float x , float y ) ;
    int getTileSizeX()const {return x_size;}
    int getTileSizeY()const {return y_size;}





};


#endif //OOP_MAP_H