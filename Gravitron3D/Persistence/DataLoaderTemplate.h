#pragma once

#include <string>

template <typename Derived>
class DataLoader {
public:
    static void saveToFile(const std::string& filename, const auto& data) {
        Derived::saveImpl(filename, data);
    }

    static void loadFromFile(const std::string& filename, auto& data) {
        Derived::loadImpl(filename, data);
    }
};