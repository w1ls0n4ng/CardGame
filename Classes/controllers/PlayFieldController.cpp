#include "GameController.h"
#include "../views/GameScene.h"
#include "PlayFieldController.h"
#include "../managers/UndoManager.h"

USING_NS_CC;

void PlayFieldController::initCoveredStatus(std::vector<CardModel>& playField)
{
    auto size = static_cast<int>(playField.size());
    // According to the rendering order, the first card in the play field is at the bottom
    for (int i = 0; i < size; ++i) {
        auto& upperCard = playField[i];

        // Check if the upperCard has covered the cards below it
        for (int j = i - 1; j >= 0; --j) {
            auto& lowerCard = playField[j];

            if (lowerCard.getBoundingBox().intersectsRect(upperCard.getBoundingBox())) {

                upperCard.addCover(lowerCard.getUid());
                lowerCard.addCoveredBy(upperCard.getUid());

                lowerCard.setInteractable(false);
            }
        }
    }
}

void PlayFieldController::handleCardClick(UndoModel& undoModel, Scene& gameScene, unsigned int cardUid)
{
    // Find the card in the play field by its UID
    auto& gameModel = GameController::gameModel;
    auto& playField = gameModel.getPlayFieldNotConst();
    auto cardModel = playField[cardUid];
    if (cardModel.getInteractable())
    {
        int clickedFace = static_cast<int>(cardModel.getFace());
        int currentHandFace = gameModel.getCurrentHandFace();
        if (clickedFace + 1 == currentHandFace ||
            clickedFace - 1 == currentHandFace)
        {
            // Make record for undo
            UndoManager::createPlayFieldRecord(gameScene, cardModel);

            // If the clicked card is adjacent to the current hand face, move it to the hand
            GameScene::movePlayFieldCardSpriteToHand(gameScene, cardUid, gameModel.getCurrentZOrder());
            revealCardThatIsNotCoveredBy(gameScene, cardModel.getUid(), cardModel.getCoverIds());

            // Modify model
            gameModel.incrementZOrder();
            gameModel.setCurrentHandFace(clickedFace);
            gameModel.decreasePlayFieldCardsLeft();

            if (gameModel.getPlayFieldCardsLeft() == 0)
            {
                GameScene::winningScene(gameScene);
            }
        }
        else
        {
            // If the clicked card is not adjacent to the current hand face, shake the card sprite
            GameScene::shakeCardSprite(gameScene, cardUid);
        }
    }
    else
    {
        GameScene::shakeCardSprite(gameScene, cardUid);
    }
}

void PlayFieldController::revealCardThatIsNotCoveredBy(Scene& gameScene, const unsigned int coveredBy,
    const std::unordered_set<unsigned int>& coverIds)
{
    auto& gameModel = GameController::gameModel;
    auto& playField = gameModel.getPlayFieldNotConst();

    for (auto& card : coverIds)
    {
        playField[card].removeCoveredBy(coveredBy);
        if (playField[card].getCoveredByIds().empty())
        {
            playField[card].setInteractable(true);
            GameScene::flipCard(gameScene, card, false);
        }
    }
}