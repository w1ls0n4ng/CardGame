#ifndef GAME_MODEL_FROM_LEVEL_GENERATOR_H
#define GAME_MODEL_FROM_LEVEL_GENERATOR_H

#include "../models/GameModel.h"
#include "../configs/models/LevelConfig.h"

class GameModelFromLevelGenerator {
public:
	// Generate a game model from the given level configuration
	// This method pupulates the game model with cards from the level configuration
	// and clears the level configuration after use.
	static void generateGameModelFromLevelConfig(LevelConfig& levelConfig);
};

#endif // GAME_MODEL_FROM_LEVEL_GENERATOR_H