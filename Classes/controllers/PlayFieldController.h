#ifndef PLAY_FIELD_CONTROLLER_H
#define PLAY_FIELD_CONTROLLER_H

#include "../models/CardModel.h"
#include "../models/UndoModel.h"
#include <vector>
#include <unordered_set>

class PlayFieldController {
public:
    static void initCoveredStatus(std::vector<CardModel>& playField);
    static void handleCardClick(UndoModel& undoModel, cocos2d::Scene& gameScene, unsigned int cardUid);

    static void revealCardThatIsNotCoveredBy(cocos2d::Scene& gameScene, const unsigned int coveredBy,
        const std::unordered_set<unsigned int>& coverIds);
};

#endif // PLAY_FIELD_CONTROLLER_H
