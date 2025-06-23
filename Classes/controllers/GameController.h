#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "../models/GameModel.h"
#include "../models/UndoModel.h"

class GameController {
public:
    static GameModel gameModel;  // Instance of the game model to manage game state
    static UndoModel undoModel;  // Instance of the undo model to manage undo actions

    // Controller method to start the game
    static void startGame();

    static void handlePlayFieldCardClick(unsigned int cardUid);
	static void handleStackCardClick(unsigned int cardUid);
    static void handleUndo();
};

#endif // GAME_CONTROLLER_H
