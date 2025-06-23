#include "../managers/UndoManager.h"
#include "../models/UndoModel.h"
#include "../controllers/GameController.h"

USING_NS_CC;

void UndoManager::createPlayFieldRecord(cocos2d::Scene& gameScene, CardModel& cardModel)
{
    UndoRecord record;
    record.actionType = UndoActionType::ROTATE_BACK_TO_PLAYFIELD;
    record.cardUid = cardModel.getUid();
    record.coveredBefore = std::vector<unsigned int>(cardModel.getCoverIds().begin(), cardModel.getCoverIds().end());
    record.previousHandFace = GameController::gameModel.getCurrentHandFace();

    // Get the previous position of the card in world coordinates
    auto layer = gameScene.getChildByName("playFieldBackground");
    auto worldVec = layer->convertToWorldSpace(Vec2(cardModel.getPosition().x, cardModel.getPosition().y));
    record.prevPositionWorldVec = worldVec;

    GameController::undoModel.pushRecord(record);
}

void UndoManager::createStackRecord(cocos2d::Scene& gameScene, CardModel& cardModel)
{
    UndoRecord record;
    record.actionType = UndoActionType::FLIP_BACK_TO_STACK;
    record.cardUid = cardModel.getUid();
    record.previousHandFace = GameController::gameModel.getCurrentHandFace();

    // Get the previous position of the card in world coordinates
    auto layer = gameScene.getChildByName("stackBackground");
    auto worldVec = layer->convertToWorldSpace(Vec2(cardModel.getPosition().x, cardModel.getPosition().y));
    record.prevPositionWorldVec = worldVec;

    GameController::undoModel.pushRecord(record);
}