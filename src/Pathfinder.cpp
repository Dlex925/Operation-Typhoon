//
// Created by dlex on 02.12.2025.
//

#include "Pathfinder.h++"
#include <queue>
#include <cmath>
#include <algorithm>
#include <iostream>

Pathfinder::Pathfinder(const Map &mapRef) : map(mapRef) {
}

float Pathfinder::calculateH(Point start, Point end) {
    return std::sqrt(std::pow(start.x - end.x, 2) + std::pow(start.y - end.y, 2));
}

bool Pathfinder::isValid(Point p) {
    if (p.x < 0 || p.y < 0 ||
        static_cast<unsigned long>(p.x) >= map.getWidth() ||
        static_cast<unsigned long>(p.y) >= map.getHeight()) {
        return false;
    }
    return !map.isWall(p.x + 0.5, p.y + 0.5);
}

std::vector<Point> Pathfinder::findPath(double startX, double startY, double endX, double endY) {
    Point start = {static_cast<int>(startX), static_cast<int>(startY)};
    Point target = {static_cast<int>(endX), static_cast<int>(endY)};

    if (start == target) return {};
    if (!isValid(target)) return {};

    std::vector<std::vector<Node> > allNodes(map.getHeight(), std::vector<Node>(map.getWidth()));
    std::vector<std::vector<bool> > closedSet(map.getHeight(), std::vector<bool>(map.getWidth(), false));
    std::vector<std::vector<bool> > openSetMap(map.getHeight(), std::vector<bool>(map.getWidth(), false));

    std::priority_queue<Node, std::vector<Node>, std::greater<> > openSet;

    Node startNode;
    startNode.pos = start;
    startNode.parent = {-1, -1};
    startNode.gCost = 0.0f;
    startNode.hCost = calculateH(start, target);
    startNode.fCost = startNode.gCost + startNode.hCost;

    openSet.push(startNode);
    allNodes[start.y][start.x] = startNode;
    openSetMap[start.y][start.x] = true;

    int dirX[] = {0, 0, 1, -1, 1, 1, -1, -1};
    int dirY[] = {1, -1, 0, 0, 1, -1, 1, -1};

    while (!openSet.empty()) {
        Node current = openSet.top();
        openSet.pop();
        openSetMap[current.pos.y][current.pos.x] = false;

        if (current.pos == target) {
            std::vector<Point> path;
            Point p = target;
            while (p != start) {
                path.push_back(p);
                p = allNodes[p.y][p.x].parent;
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        closedSet[current.pos.y][current.pos.x] = true;

        for (int i = 0; i < 8; ++i) {
            Point neighborPos = {current.pos.x + dirX[i], current.pos.y + dirY[i]};

            if (!isValid(neighborPos)) continue;
            if (closedSet[neighborPos.y][neighborPos.x]) continue;

            float moveCost = (dirX[i] != 0 && dirY[i] != 0) ? 1.414f : 1.0f;
            float newGCost = current.gCost + moveCost;

            bool inOpenSet = openSetMap[neighborPos.y][neighborPos.x];

            if (!inOpenSet || newGCost < allNodes[neighborPos.y][neighborPos.x].gCost) {
                Node neighborNode;
                neighborNode.pos = neighborPos;
                neighborNode.parent = current.pos;
                neighborNode.gCost = newGCost;
                neighborNode.hCost = calculateH(neighborPos, target);
                neighborNode.fCost = neighborNode.gCost + neighborNode.hCost;

                allNodes[neighborPos.y][neighborPos.x] = neighborNode;

                if (!inOpenSet) {
                    openSet.push(neighborNode);
                    openSetMap[neighborPos.y][neighborPos.x] = true;
                }
            }
        }
    }

    return {};
}
