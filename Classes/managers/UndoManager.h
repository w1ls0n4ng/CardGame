#ifndef UNDO_MANAGER_H
#define UNDO_MANAGER_H

#include "../models/CardModel.h"
#include "../models/UndoModel.h"
#include "cocos2d.h"

class UndoManager {
public:
	static void createPlayFieldRecord(cocos2d::Scene& gameScene, CardModel& cardModel);
	static void createStackRecord(cocos2d::Scene& gameScene, CardModel& cardModel);
};

#endif // UNDO_MANAGER_H
