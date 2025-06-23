#include "GameScene.h"
#include "../controllers/GameController.h"
#include "../configs/models/CardResConfig.h"
#include "../configs/models/Params.h"
#include "../../cocos2d/cocos/audio/include/AudioEngine.h"

using namespace cocos2d::experimental;
USING_NS_CC;

Scene* GameScene::createScene() {
    return GameScene::create();
}

bool GameScene::init() {
    if (!Scene::init()) {
        return false;
    }

    // Get the visible screen size
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // Create the play field background
    auto playFieldBackground = LayerColor::create(Color4B(244, 225, 194, 255));
    playFieldBackground->setName("playFieldBackground");
    playFieldBackground->setContentSize(Size(1080, 1500));
    playFieldBackground->setPosition(Vec2(origin.x, origin.y + 580));
    this->addChild(playFieldBackground, 0);

    // Create the stack area background
    auto stackBackground = LayerColor::create(Color4B(238, 201, 183, 255));
    stackBackground->setName("stackBackground");
    stackBackground->setContentSize(Size(1080, 580));
    stackBackground->setPosition(Vec2::ZERO);
    this->addChild(stackBackground, -1);

    // Create the undo label
    auto undoLabel = Label::createWithTTF("Undo", "fonts/Marker Felt.ttf", 50);
    auto undoItem = MenuItemLabel::create(undoLabel, [](Ref* sender) {
        // Call controller method to handle undo logic
        GameController::handleUndo();
        });

    // Position the button in the bottom-right corner
    undoItem->setPosition(Vec2(origin.x + visibleSize.width - 100, origin.y + 275));

    // Create the menu and add it to the scene
    auto menu = Menu::create(undoItem, nullptr);
    menu->setPosition(Vec2::ZERO); // Use absolute positioning
    this->addChild(menu, 1);

    AudioEngine::play2d("RisingStar.mp3", true);

    return true;
}

void GameScene::helperAddCardRes(Sprite& cardSprite, const std::string& suitRes,
    const std::string& centerRes, const std::string& cornerRes)
{
    auto suitSprite = Sprite::create(suitRes);
    suitSprite->setPosition(Vec2(140, 240));
    cardSprite.addChild(suitSprite);


    auto centerSprite = Sprite::create(centerRes);
    centerSprite->setPosition(Vec2(90, 120));
    cardSprite.addChild(centerSprite);

    auto cornerSprite = Sprite::create(cornerRes);
    cornerSprite->setPosition(Vec2(30, 240));
    cardSprite.addChild(cornerSprite);
}

void GameScene::initPlayFieldDecks(Scene& gameScene)
{
    // Get the play field layer
    auto& gameModel = GameController::gameModel;
    auto& gameArea = gameModel.getPlayFieldNotConst();
    auto playFieldLayer = gameScene.getChildByName("playFieldBackground");

    // Create card sprites for each card in the play field
    for (auto& cardModel : gameArea) {
        // Create the base card sprite
        auto cardSprite = Sprite::create("card_general.png");
        cardSprite->setTag(cardModel.getUid());
        cardSprite->setPosition(Vec2(cardModel.getPosition().x, cardModel.getPosition().y));
        playFieldLayer->addChild(cardSprite);

        // Save the bounding box to the model (used for overlap detection)
        auto boundingBox = cardSprite->getBoundingBox();
        cardModel.setBoundingBox(boundingBox);

        // Add card UI components
        std::string suitRes, centerRes, cornerRes;
        CardResConfig::getCardRes(cardModel.getFace(), cardModel.getSuit(), suitRes, centerRes, cornerRes);
        GameScene::helperAddCardRes(*cardSprite, suitRes, centerRes, cornerRes);

        // Add a touch event listener to detect clicks on the card
        auto listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true); // Prevent touch propagation

        listener->onTouchBegan = [=, &cardModel](Touch* touch, Event* event) {
            // Convert touch point to sprite's local space
            auto locationInNode = cardSprite->convertToNodeSpace(touch->getLocation());
            auto size = cardSprite->getContentSize();
            Rect rect(0, 0, size.width, size.height);

            // Check if the touch is inside the sprite
            if (rect.containsPoint(locationInNode)) {
                // Call the controller to handle card click logic
                GameController::handlePlayFieldCardClick(cardModel.getUid());
                return true; // Swallow the touch
            }
            return false; // Let the touch pass through
            };

        // Register the listener with the event dispatcher
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, cardSprite);
    }
}


void GameScene::initStackDecks(Scene& gameScene)
{
    // Get the stack layer and add cards to the stack layer
    auto& gameModel = GameController::gameModel;
    auto& stack = gameModel.getStackNotConst();
    auto stackLayer = gameScene.getChildByName("stackBackground");

    // Get the stack size and calculate card spacing, limit it to a maximum of 100 pixels
    auto stackSize = stack.size();

    // Create card sprites for each card in the stack and position them
    int cardOffset = 0;
    for (auto& cardModel : stack) {
        auto cardSprite = Sprite::create("card_general.png");

        // If the card is the last one in the stack, position it at the hand position
        if (cardOffset == stackSize - 1)
            cardSprite->setPosition(K_HAND_POSITION);
        else
            cardSprite->setPosition(Vec2(cardModel.getPosition().x, cardModel.getPosition().y));
        cardOffset++;

        cardSprite->setTag(cardModel.getUid());
        stackLayer->addChild(cardSprite, cardOffset);

        // Set the bounding box for the card sprite based on its position and size;
        auto boundingBox = cardSprite->getBoundingBox();
        cardModel.setBoundingBox(boundingBox);

        std::string suitRes, centerRes, cornerRes;
        CardResConfig::getCardRes(cardModel.getFace(), cardModel.getSuit(), suitRes, centerRes, cornerRes);
        GameScene::helperAddCardRes(*cardSprite, suitRes, centerRes, cornerRes);

        // Add a touch event listener to detect clicks on the card
        auto listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true); // Prevent touch propagation

        listener->onTouchBegan = [=, &cardModel](Touch* touch, Event* event) {
            // Convert touch point to sprite's local space
            auto locationInNode = cardSprite->convertToNodeSpace(touch->getLocation());
            auto size = cardSprite->getContentSize();
            Rect rect(0, 0, size.width, size.height);

            // Check if the touch is inside the sprite
            if (rect.containsPoint(locationInNode)) {
                // Call the controller to handle card click logic
                GameController::handleStackCardClick(cardModel.getUid());
                return true; // Swallow the touch
            }
            return false; // Let the touch pass through
            };

        // Register the listener with the event dispatcher
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, cardSprite);
    }
}

void GameScene::initHideCardSprite(Scene& gameScene)
{
    // Get the play field layer and stack layer
    auto& gameModel = GameController::gameModel;
    auto playFieldLayer = gameScene.getChildByName("playFieldBackground");
    auto stackLayer = gameScene.getChildByName("stackBackground");
    
    // Hide all card sprites that are not interactable in the play field
    for (auto& cardModel : gameModel.getPlayFieldNotConst()) {
        if (!cardModel.getInteractable())
        {
            auto cardSprite = playFieldLayer->getChildByTag(cardModel.getUid());
            auto backsideSprite = Sprite::create("card_general.png");
            backsideSprite->setName("BacksideSprite");
            backsideSprite->setAnchorPoint(Vec2::ZERO);
            cardSprite->addChild(backsideSprite);
        }
    }

    // Hide all card sprites except the last one in the stack
    auto stack = gameModel.getStackNotConst();
    ssize_t stackSize = stack.size();
    for (ssize_t i = 0; i < stackSize - 1; ++i) {
        auto cardSprite = stackLayer->getChildByTag(stack[i].getUid());
        auto backsideSprite = Sprite::create("card_general.png");
        backsideSprite->setName("BacksideSprite");
        backsideSprite->setAnchorPoint(Vec2::ZERO);
        cardSprite->addChild(backsideSprite);
    }
}

void GameScene::shakeCardSprite(Scene& gameScene, unsigned int cardUid)
{
    // Find the card sprite by its UID
     auto layer = gameScene.getChildByName("playFieldBackground");

    auto cardSprite = layer->getChildByTag(cardUid);
    if (cardSprite) {
        // Create a shake action
        auto shakeAction = Sequence::create(
            MoveBy::create(0.1f, Vec2(10, 0)), // Move right
            MoveBy::create(0.1f, Vec2(-20, 0)), // Move left
            MoveBy::create(0.1f, Vec2(20, 0)), // Move right again
            MoveBy::create(0.1f, Vect(-10, 0)), // Move back to the original position
            nullptr
        );
        cardSprite->runAction(shakeAction);
    }

    AudioEngine::play2d("error.mp3");
}

void GameScene::movePlayFieldCardSpriteToHand(Scene& gameScene, unsigned int cardUid, int zOrder)
{
    // Find the card sprite by its UID
    auto playFieldLayer = gameScene.getChildByName("playFieldBackground");
    auto stackLayer = gameScene.getChildByName("stackBackground");
	// Get the world hand position and convert it to the play field's local space
    auto posToPlayField = playFieldLayer->convertToNodeSpace(K_HAND_POSITION);
    auto cardSprite = playFieldLayer->getChildByTag(cardUid);

    // Create rotation action
    auto rotate = RotateBy::create(0.3f, 360); // Rotate 360 degrees in 0.3s

    // Create move action
    auto move = MoveTo::create(0.5f, posToPlayField); // Move to hand position

    // Combine the actions: first rotate, then move (or use Spawn if you want simultaneous)
    auto spawn = Spawn::createWithTwoActions(rotate, move);

    // After animation: move cardSprite to stackLayer
    auto moveToStackLayer = CallFunc::create([=]() {
        // Remove from current parent, retain to avoid auto-release
        cardSprite->retain();
        cardSprite->removeFromParentAndCleanup(false);
        cardSprite->setPosition(K_HAND_POSITION);
        stackLayer->addChild(cardSprite, zOrder);
        cardSprite->release();
        });

    // Create the full animation sequence
    auto sequence = Sequence::create(spawn, moveToStackLayer, nullptr);
    cardSprite->runAction(sequence);

    // Run the animation
    cardSprite->runAction(spawn);

    AudioEngine::play2d("correct.mp3");
}

void GameScene::flipCard(Scene& gameScene, unsigned int cardUid, bool flipToBack)
{
    // Get the play field layer and card sprite
    auto playFieldLayer = gameScene.getChildByName("playFieldBackground");
    auto cardSprite = playFieldLayer->getChildByTag(cardUid);

    // First half rotation (scale X down to 0)
    auto scaleDown = ScaleTo::create(0.15f, 0.0f, 1.0f);

	auto backsideSprite = cardSprite->getChildByName("BacksideSprite");

    // Callback to reveal or hid
    auto callback = CallFunc::create([=]() {
        backsideSprite->setVisible(flipToBack);
        });

    // Second half rotation (scale X back to 1.0)
    auto scaleUp = ScaleTo::create(0.15f, 1.0f, 1.0f);

    // Sequence of flip
    auto flipSequence = Sequence::create(scaleDown, callback, scaleUp, nullptr);
    cardSprite->runAction(flipSequence);
}

void GameScene::moveStackCardSprite(Scene& gameScene, unsigned int cardUid, int zOrder, Vec2 newPosition, bool moveToStack)
{
    auto stackLayer = gameScene.getChildByName("stackBackground");
    auto cardSprite = stackLayer->getChildByTag(cardUid);
    cardSprite->setLocalZOrder(zOrder); // Set the Z-order so that it appears on top in the stack

    // Flip card (scaleX down, remove back, scaleX up)
    auto scaleDown = ScaleTo::create(0.15f, 0.0f, 1.0f);
    auto backsideSprite = cardSprite->getChildByName("BacksideSprite");
    auto reveal = CallFunc::create([=]() {
        backsideSprite->setVisible(moveToStack);
        });
    auto scaleUp = ScaleTo::create(0.15f, 1.0f, 1.0f);
    auto flipSequence = Sequence::create(scaleDown, reveal, scaleUp, nullptr);

    // Move to _kHandPosition + rotate
    auto move = MoveTo::create(0.5f, newPosition);

    auto sequence = Sequence::create(flipSequence, move, nullptr);
    cardSprite->runAction(sequence);

    AudioEngine::play2d("correct.mp3");
}

void GameScene::undoBackToPlayField(Scene& gameScene, unsigned int cardUid, int zOrder, const cocos2d::Vec2& prevPositionWorldVec)
{
    auto playFieldLayer = gameScene.getChildByName("playFieldBackground");
    auto stackLayer = gameScene.getChildByName("stackBackground");
    auto cardSprite = stackLayer->getChildByTag(cardUid);

    // Remove the card from the stack layer and reattach to playFieldLayer
    auto relativePos = playFieldLayer->convertToNodeSpace(K_HAND_POSITION);
    cardSprite->retain();
    cardSprite->removeFromParentAndCleanup(false);
    cardSprite->setPosition(relativePos);
    playFieldLayer->addChild(cardSprite, zOrder);
    cardSprite->release();

    // Run move + rotate animation while still in stackLayer
    relativePos = playFieldLayer->convertToNodeSpace(prevPositionWorldVec);
    auto move = MoveTo::create(0.5f, relativePos);
    auto rotate = RotateBy::create(0.3f, -360);
    auto spawn = Spawn::createWithTwoActions(move, rotate);

    auto sequence = Sequence::create(spawn, nullptr);
    cardSprite->runAction(sequence);

    AudioEngine::play2d("correct.mp3");
}

void GameScene::winningScene(Scene& gameScene)
{
    // Create a semi-transparent overlay
    auto overlay = LayerColor::create(Color4B(0, 0, 0, 180));
    overlay->setContentSize(Director::getInstance()->getVisibleSize());
    overlay->setPosition(Vec2::ZERO);
    gameScene.addChild(overlay, 2000); // high z-order to cover everything

    // Create "YOU WIN" label
    auto winLabel = Label::createWithTTF("YOU WIN!", "fonts/Marker Felt.ttf", 72);
    winLabel->setPosition(Director::getInstance()->getVisibleSize() / 2 + Size(0, 200));
    overlay->addChild(winLabel);

    // Create "Play Again" button
    auto playAgainLabel = Label::createWithTTF("Play Again", "fonts/Marker Felt.ttf", 40);
    auto playAgainItem = MenuItemLabel::create(playAgainLabel, [](Ref* sender) {
        // Restart game or go to level selection
        Director::getInstance()->replaceScene(GameScene::create()); // restart GameScene
        });

    // Create "Exit" button
    auto exitLabel = Label::createWithTTF("Exit", "fonts/Marker Felt.ttf", 40);
    auto exitItem = MenuItemLabel::create(exitLabel, [](Ref* sender) {
        Director::getInstance()->end(); // Close app
        });

    // Create menu with both buttons
    auto menu = Menu::create(playAgainItem, exitItem, nullptr);
    menu->alignItemsVerticallyWithPadding(30);
    menu->setPosition(Director::getInstance()->getVisibleSize() / 2 - Size(0, 100));
    overlay->addChild(menu);
}
