#pragma once

#include "../Model/Settings.h"

class SettingsDataLoader {
public:
    static void saveToFile(const std::string& filename, const Settings& settings);

    static Settings loadFromFile(const std::string& filename);
};