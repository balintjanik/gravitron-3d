#pragma once

#include <string>

template <typename Derived>
class DataLoader {
public:
    template <typename T>
    static void saveToFile(const std::string& filename, const T& data) {
        Derived::saveImpl(filename, data);
    }

    template <typename T>
    static void loadFromFile(const std::string& filename, T& data) {
        Derived::loadImpl(filename, data);
    }
};