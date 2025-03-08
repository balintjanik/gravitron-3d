#include "SettingsDataLoader.h"
#include <fstream>
#include <iostream>

void SettingsDataLoader::saveToFile(const std::string& filename, const Settings& settings) {
    std::cout << "Saving settings to " << filename << std::endl;
    // TODO: add actual save logic
}

Settings SettingsDataLoader::loadFromFile(const std::string& filename) {
    std::cout << "Loading settings from " << filename << std::endl;
    // TODO: add actual load logic
    return Settings();
}