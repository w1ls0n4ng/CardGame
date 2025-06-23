#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "../models/GameModel.h"

class GameScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    // Initialize the game scene
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);

    // Initialize the card decks in the game scene
    static void initPlayFieldDecks(Scene& gameScene);
    static void initStackDecks(Scene& gameScene);

    // Hide the card sprite that is not interactable
    static void initHideCardSprite(Scene& gameScene);

    // Shake the card sprite to indicate it is not interactable
    static void shakeCardSprite(Scene& gameScene, unsigned int cardUid);

    // Move the play field card sprite to the hand position
    static void movePlayFieldCardSpriteToHand(Scene& gameScene, unsigned int cardUid, int zOrder);
    // Move the stack card sprite to the hand position
    static void moveStackCardSprite(Scene& gameScene, unsigned int cardUid, int zOrder, cocos2d::Vec2 newPosition, bool moveToStack);

    // Flip the card sprite
    // @param flipToFront: true to flip to front, false to flip to back
    static void flipCard(Scene& gameScene, unsigned int cardUid, bool flipToBack);

    // Play the undo action back to the play field
    static void undoBackToPlayField(Scene& gameScene, unsigned int cardUid, const cocos2d::Vec2 & prevPositionWorldVec);

    static void winningScene(Scene& gameScene);

    // Helper function to add card reesources to a card sprite
    static void helperAddCardRes(cocos2d::Sprite& cardSprite, const std::string& suitRes,
        const std::string& centerRes, const std::string& cornerRes);
};

#endif // __GAME_SCENE_H__
