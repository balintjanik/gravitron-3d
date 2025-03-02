#pragma once

#include "DataLoaderTemplate.h"
#include <map>
#include <string>

class SettingsDataLoader : public DataLoader<SettingsDataLoader> {
public:
    static void saveImpl(const std::string& filename, const std::map<std::string, std::string>& settings);
    static void loadImpl(const std::string& filename, std::map<std::string, std::string>& settings);
};