#include "GameController.h"
#include "PlayFieldController.h"
#include "StackController.h"
#include "../views/GameScene.h"
#include "../configs/loaders/LevelConfigLoader.h"
#include "../configs/models/LevelConfig.h"
#include "../services/GameModelFromLevelGenerator.h"

USING_NS_CC;

GameModel GameController::gameModel; // Initialize the static game model instance
UndoModel GameController::undoModel; // Initialize the static undo model intance

void GameController::startGame() {
	// Create the game scene
	auto gameScene = GameScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, gameScene));

	// Load the level configuration from a JSON file
	LevelConfig levelConfig;
	if (LevelConfigLoader::loadLevelConfig("TestLevel.json", levelConfig)) {
		// Successfully loaded level configuration
		CCLOG("Level configuration loaded successfully.");
	} else {
		// Failed to load level configuration
		CCLOG("Failed to load level configuration.");
	}

	// Load the level configuration into the game model
	GameModelFromLevelGenerator::generateGameModelFromLevelConfig(levelConfig);

	// Initialize the card decks in the game scene
	GameScene::initPlayFieldDecks(*gameScene);
	GameScene::initStackDecks(*gameScene);

	// Initialize the covered by and cover statuses for the play field and stack
	PlayFieldController::initCoveredStatus(gameModel.getPlayFieldNotConst());
	StackController::initCoveredStatus(gameModel.getStackNotConst());

	GameScene::initHideCardSprite(*gameScene);
}

void GameController::handlePlayFieldCardClick(unsigned int cardUid) {
	auto gameScene = Director::getInstance()->getRunningScene();
	PlayFieldController::handleCardClick(undoModel, *gameScene, cardUid);
}

void GameController::handleStackCardClick(unsigned int cardUid) {
	auto gameScene = Director::getInstance()->getRunningScene();
	StackController::handleCardClick(*gameScene, cardUid);
}

void GameController::handleUndo() {
	if (!undoModel.hasUndo())
		return;

	UndoRecord record = undoModel.topRecord();
	auto gameScene = Director::getInstance()->getRunningScene();

	gameModel.setCurrentHandFace(record.previousHandFace);

	if (record.actionType == UndoActionType::ROTATE_BACK_TO_PLAYFIELD)
	{
		gameModel.incrementPlayFieldCardsLeft();
		std::vector<unsigned int> cardsToFlipBack;
		auto playField = gameModel.getPlayFieldNotConst();
		for (auto& cardUid : record.coveredBefore)
		{
			auto cardModel = playField[cardUid];
			cardModel.setInteractable(false);

			// If it is not covered by any cards add it to the flipBack vector
			if (cardModel.getCoveredByIds().empty())
			{
				cardsToFlipBack.push_back(cardUid);
			}
			cardModel.addCoveredBy(record.cardUid);
		}
		// Move the card back to the play field
		GameScene::undoBackToPlayField(*gameScene, record.cardUid, record.prevPositionWorldVec);
		for (auto& cardUid : cardsToFlipBack)
		{
			GameScene::flipCard(*gameScene, cardUid, true);
		}
	}
	else
	{
		auto& gameModel = GameController::gameModel;
		GameScene::moveStackCardSprite(*gameScene, record.cardUid, gameModel.getCurrentZOrder(), record.prevPositionWorldVec, true);
		gameModel.incrementZOrder();

		// Set the card to be undo as interactable
		auto& stack = gameModel.getStackNotConst();
		ssize_t playFieldSize = gameModel.getPlayFieldNotConst().size();
		auto& cardModel = stack[record.cardUid - playFieldSize];
		cardModel.setInteractable(true);

		// Set the card before it as uninteractable
		int previousCardIndex = record.cardUid - playFieldSize - 1;
		if (previousCardIndex >= 0)
		{
			auto& previousCardModel = stack[previousCardIndex];
			previousCardModel.setInteractable(false);
		}
	}

	undoModel.popRecord();
}