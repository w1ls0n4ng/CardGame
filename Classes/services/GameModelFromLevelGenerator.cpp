#include "GameModelFromLevelGenerator.h"
#include "../controllers/GameController.h"
#include "../configs/models/Params.h"

void GameModelFromLevelGenerator::generateGameModelFromLevelConfig(LevelConfig& levelConfig)
{
	// Load play field cards from the level configuration
	auto& gameModel = GameController::gameModel;
	auto playFieldCards = levelConfig.getPlayFieldCards();
	for (const auto& cardConfig : playFieldCards)
	{
		auto cardModel = CardModel(gameModel.getUid(), cardConfig);
		gameModel.incrementUid();
		gameModel.addCardToPlayfield(cardModel);
	}

	// Set how many cards are the in the play field
	gameModel.setPlayFieldCardsLeft(playFieldCards.size());

	// Load stack cards from the level configuration
	auto stackCards = levelConfig.getStackCards();
	for (const auto& cardConfig : stackCards)
	{
		auto cardModel = CardModel(gameModel.getUid(), cardConfig);
		gameModel.incrementUid();
		gameModel.addCardToStack(cardModel);
	}

	auto face = gameModel.getStackNotConst().back().getFace();
	gameModel.setCurrentHandFace(static_cast<int>(face));

	// Set stack position
	auto &stack = gameModel.getStackNotConst();
	auto stackSize = stack.size();
	int cardSpacing = K_STACK_FIELD_SIZE / stackSize;
	if (cardSpacing > K_STACK_MAX_SPACING)
	{
		cardSpacing = K_STACK_MAX_SPACING;
	}

	int offset = 0;
	for (auto& cardModel : stack)
	{
		cardModel.setPosition(K_STARTING_X_STACK_FIELD + offset * cardSpacing, K_STARTING_Y_STACK_FIELD);
		offset++;
	}

	// Clear the level configuration after generating the game model
	levelConfig.clearCards();
}