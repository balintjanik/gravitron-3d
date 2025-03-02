#include "SettingsDataLoader.h"
#include <fstream>
#include <iostream>

void SettingsDataLoader::saveImpl(const std::string& filename, const std::map<std::string, std::string>& settings) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Error: Unable to open settings file for writing: " << filename << std::endl;
        return;
    }

    throw "saveImpl for SettingsDataLoader is not yet implemented!";
}

void SettingsDataLoader::loadImpl(const std::string& filename, std::map<std::string, std::string>& settings) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error: Unable to open settings file for reading: " << filename << std::endl;
        return;
    }

    throw "loadImpl for SettingsDataLoader is not yet implemented!";
}