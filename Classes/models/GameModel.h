#ifndef GAME_MODEL_H
#define GAME_MODEL_H

#include "CardModel.h"
#include <vector>

class GameModel
{
private:
    unsigned int _uid;                        // Unique identifier for all the cards in the game model
    std::vector<CardModel> _playFieldCards;   // Cards in the playfield (main area)
    std::vector<CardModel> _stackCards;       // Cards in the stack (hand)
    int currentHandFace;                      // Current face of the hand
	int currentZOrder = 10;                   // Current Z-order for rendering at hand position
    int playFieldCardsLeft;

public:
    // Getters
    std::vector<CardModel>& getPlayFieldNotConst() { return _playFieldCards; }
    std::vector<CardModel>& getStackNotConst() { return _stackCards; }
    unsigned int getUid() const { return _uid; }
    int getCurrentHandFace() const { return currentHandFace; }
    int getCurrentZOrder() const { return currentZOrder; }
    int getPlayFieldCardsLeft() const { return playFieldCardsLeft; }

    // Setters and Increment methods
    void setCurrentHandFace(int face) { currentHandFace = face;  }
    void setPlayFieldCardsLeft(int left) { playFieldCardsLeft = left;  }
    void incrementUid() { _uid++; }
    void incrementZOrder() { currentZOrder++; }
    void incrementPlayFieldCardsLeft() { playFieldCardsLeft++; }
    void decreasePlayFieldCardsLeft() { playFieldCardsLeft--; }

    // Interface methods for adding cards to the playfield or stack
    void addCardToPlayfield(const CardModel& card) { _playFieldCards.push_back(card); }
    void addCardToStack(const CardModel& card) { _stackCards.push_back(card); }
};

#endif // GAME_MODEL_H