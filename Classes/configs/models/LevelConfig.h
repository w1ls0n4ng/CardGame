#ifndef LEVEL_CONFIG_H
#define LEVEL_CONFIG_H

#include <vector>
#include "cocos2d.h"

// Card suit types
enum CardSuitType
{
    CST_NONE = -1,    // No suit
    CST_CLUBS,        // Clubs
    CST_DIAMONDS,     // Diamonds
    CST_HEARTS,       // Hearts
    CST_SPADES,       // Spades
    CST_NUM_CARD_SUIT_TYPES  // Total number of suit types
};

// Card face types
enum CardFaceType
{
    CFT_NONE = -1,    // No face
    CFT_ACE,          // Ace
    CFT_TWO,          // Two
    CFT_THREE,        // Three
    CFT_FOUR,         // Four
    CFT_FIVE,         // Five
    CFT_SIX,          // Six
    CFT_SEVEN,        // Seven
    CFT_EIGHT,        // Eight
    CFT_NINE,         // Nine
    CFT_TEN,          // Ten
    CFT_JACK,         // Jack
    CFT_QUEEN,        // Queen
    CFT_KING,         // King
    CFT_NUM_CARD_FACE_TYPES  // Total number of face types
};

// Define the position of a card
struct CardPosition {
    int x; // x-coordinate
    int y; // y-coordinate

    CardPosition(int xPos = 0, int yPos = 0) : x(xPos), y(yPos) {}
};

// Define the structure of a card
struct CardConfig {
    CardFaceType cardFace;  // Face value of the card
    CardSuitType cardSuit;  // Suit of the card
    CardPosition position;  // Position on screen

    CardConfig(CardFaceType face = CFT_NONE, CardSuitType suit = CST_NONE, int x = 0, int y = 0)
        : cardFace(face), cardSuit(suit), position(x, y) {}
};

// Class to hold level configuration
class LevelConfig {
private:
    std::vector<CardConfig> _playFieldCards;  // Cards in the playfield (main area)
    std::vector<CardConfig> _stackCards;      // Cards in the stack (hand)
public:
    // Interface methods for adding cards to the playfield or stack
    void addCardToPlayfield(const CardConfig& card) {
        _playFieldCards.push_back(card);
    }
    void addCardToStack(const CardConfig& card) {
        _stackCards.push_back(card);
    }

    // Getters for playfield and stack cards
    std::vector<CardConfig>& getPlayFieldCards() const {
		return const_cast<std::vector<CardConfig>&>(_playFieldCards);
    }
    std::vector<CardConfig>& getStackCards() const {
        return const_cast<std::vector<CardConfig>&>(_stackCards);
    }

    // Clear all cards from playfield and stack after loading into game model
    void clearCards() {
        _playFieldCards.clear();
        _stackCards.clear();
    }
};

#endif // LEVEL_CONFIG_H
