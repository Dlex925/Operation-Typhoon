//
// Created by dlex on 18.11.2025.
//

#ifndef OOP_EXCEPTIONS_H
#define OOP_EXCEPTIONS_H
#include <stdexcept>
#include <string>

class GameException : public std::runtime_error {
public:
    explicit GameException(const std::string &message)
        : std::runtime_error("Game Error: " + message) {
    }
};

class AssetLoadException : public GameException {
public:
    explicit AssetLoadException(const std::string &filename, const std::string &type = "Asset")
        : GameException("Failed to load " + type + " from file: " + filename) {
    }
};


class FontException : public AssetLoadException {
public:
    explicit FontException(const std::string &filename)
        : AssetLoadException(filename, "Font") {
    }
};

class MapException : public GameException {
public:
    explicit MapException(const std::string &details)
        : GameException("Map Error: " + details) {
    }
};

class UnknownEnemyTypeException : public GameException {
public:
    explicit UnknownEnemyTypeException(int typeCode)
        : GameException("Unknown Enemy Type Code detected: " + std::to_string(typeCode)) {
    }
};


#endif //OOP_EXCEPTIONS_H
