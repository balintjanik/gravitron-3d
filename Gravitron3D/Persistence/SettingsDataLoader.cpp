#include "SettingsDataLoader.h"
#include <fstream>
#include <iostream>

void SettingsDataLoader::saveToFile(const std::string& filename, const Settings& settings) {
    std::ofstream out(filename, std::ios::binary);
    if (!out) {
        throw std::runtime_error("Failed to open file for saving: " + filename);
    }

    out.write(reinterpret_cast<const char*>(&settings), sizeof(Settings));
    if (!out) {
        throw std::runtime_error("Failed to write settings data.");
    }

    out.close();
}

Settings SettingsDataLoader::loadFromFile(const std::string& filename) {
    Settings settings;
    std::ifstream in(filename, std::ios::binary);
    if (!in) {
        throw std::runtime_error("Failed to open file for loading: " + filename);
    }

    in.read(reinterpret_cast<char*>(&settings), sizeof(Settings));
    if (!in) {
        throw std::runtime_error("Failed to read settings data.");
    }

    in.close();
    return settings;
}