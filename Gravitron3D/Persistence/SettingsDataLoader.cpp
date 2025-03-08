#include "SettingsDataLoader.h"
#include <fstream>
#include <iostream>

void SettingsDataLoader::saveToFile(const std::string& filename, const Settings& settings) {
    std::cout << "Saving settings to " << filename << std::endl;
    
    std::ofstream out(filename, std::ios::binary);
    if (!out) {
        std::cerr << "Failed to open file for saving: " << filename << std::endl;
        return;
    }
    out.write(reinterpret_cast<const char*>(&settings), sizeof(Settings));
    out.close();
}

Settings SettingsDataLoader::loadFromFile(const std::string& filename) {
    std::cout << "Loading settings from " << filename << std::endl;
    
    Settings settings;
    std::ifstream in(filename, std::ios::binary);
    if (!in) {
        std::cerr << "Failed to open file for loading: " << filename << std::endl;
        return settings; // Return default settings if loading fails
    }
    in.read(reinterpret_cast<char*>(&settings), sizeof(Settings));
    in.close();
    return settings;
}