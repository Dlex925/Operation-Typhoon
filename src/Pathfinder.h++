//
// Created by dlex on 02.12.2025.
//

#ifndef OOP_PATHFINDER_H
#define OOP_PATHFINDER_H

#include <vector>
#include <list>
#include <utility>
#include "Map.h++"

struct Point {
    int x, y;
    bool operator==(const Point& other) const { return x == other.x && y == other.y; }
    bool operator!=(const Point& other) const { return !(*this == other); }
};

class Pathfinder {
    struct Node {
        Point pos;
        Point parent;
        float gCost;
        float hCost;
        float fCost;

        bool operator>(const Node& other) const {
            return fCost > other.fCost;
        }
    };

    const Map& map;
    std::vector<std::vector<bool>> visited;

    static float calculateH(Point start, Point end);
    bool isValid(Point p);

public:
    explicit Pathfinder(const Map& mapRef);
    std::vector<Point> findPath(double startX, double startY, double endX, double endY);
};

#endif //OOP_PATHFINDER_H