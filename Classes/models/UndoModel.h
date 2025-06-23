#ifndef UNDO_MODEL_H
#define UNDO_MODEL_H

#include "cocos2d.h"

enum class UndoActionType {
    ROTATE_BACK_TO_PLAYFIELD,
    FLIP_BACK_TO_STACK,
};

// Record of a single undoable action
struct UndoRecord {
    UndoActionType actionType;
    unsigned int cardUid;                      // The card being acted upon
    int previousHandFace;                      // Previous hand face before the action
    cocos2d::Vec2 prevPositionWorldVec;        // Previous position before the action
    std::vector<unsigned int> coveredBefore;   // Covered cards before the action
};


class UndoModel {
private:
    std::stack<UndoRecord> undoStack; // Stack to hold undo records

public:
    // Add a new undo record to the stack
    void pushRecord(const UndoRecord& record) { undoStack.push(record); };

    // Check if there is any undo record
    bool hasUndo() const { return !undoStack.empty(); };

    // Get the top record
    UndoRecord topRecord() const { return undoStack.top(); };

    // Pop the top record
    void popRecord() { undoStack.pop(); };
};

#endif // UNDO_MODEL_H
