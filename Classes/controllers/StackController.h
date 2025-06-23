#ifndef STACK_CONTROLLER_H
#define STACK_CONTROLLER_H

#include "../models/CardModel.h"
#include <vector>

class StackController {
public:
    static void initCoveredStatus(std::vector<CardModel>& stack);
    static void handleCardClick(cocos2d::Scene& gameScene, unsigned int cardUid);
};

#endif // STACK_CONTROLLER_H
