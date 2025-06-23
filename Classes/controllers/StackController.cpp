#include "StackController.h"
#include "GameController.h"
#include "../views/GameScene.h"
#include "../managers/UndoManager.h"
#include "../configs/models/Params.h"

void StackController::initCoveredStatus(std::vector<CardModel>& stack)
{
	auto size = stack.size();
	// Set all cards in the stack except the second last one
	for (size_t i = 0; i < size - 2; ++i) {
        stack[i].setInteractable(false);
	}

    stack[size - 1].setInteractable(false);
}

void StackController::handleCardClick(cocos2d::Scene& gameScene, unsigned int cardUid)
{
    // Find the card in the play field by its UID
    auto& gameModel = GameController::gameModel;
    auto& stack = gameModel.getStackNotConst();
    ssize_t playFieldSize = gameModel.getPlayFieldNotConst().size();
    ssize_t stackSize = stack.size();
    auto& cardModel = stack[cardUid - playFieldSize];
    if (cardModel.getInteractable())
    {
        // Create stack record for undo
        UndoManager::createStackRecord(gameScene, cardModel);

        // Call the view layer
        GameScene::moveStackCardSprite(gameScene, cardUid, gameModel.getCurrentZOrder(), K_HAND_POSITION, false);
        gameModel.incrementZOrder();
        gameModel.setCurrentHandFace(static_cast<int>(cardModel.getFace()));
        
        cardModel.setInteractable(false);
        // Set previous card in the stack to interactable
        int previousCardIndex = cardUid - playFieldSize - 1;
        if (previousCardIndex >= 0)
        {
            stack[previousCardIndex].setInteractable(true);
        }
    }
}