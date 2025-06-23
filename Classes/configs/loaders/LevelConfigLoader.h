#ifndef LEVEL_CONFIG_LOADER_H
#define LEVEL_CONFIG_LOADER_H

#include "../models/LevelConfig.h"
#include "cocos2d.h"

class LevelConfigLoader {
public:
    // Load the configuration from json file for the given level
    static bool loadLevelConfig(const std::string& filePath, LevelConfig& config);
};

#endif // LEVEL_CONFIG_LOADER_H
