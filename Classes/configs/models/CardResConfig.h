#ifndef CARD_RES_CONFIG_H
#define CARD_RES_CONFIG_H

#include "LevelConfig.h"

// Const initialization of the suit resources
const std::string _cardSuitRes[CST_NUM_CARD_SUIT_TYPES] = {
    "club.png",    // CST_CLUBS
    "diamond.png", // CST_DIAMONDS
    "heart.png",   // CST_HEARTS
    "spade.png",   // CST_SPADES
};

// Const initialization of the face resources
const std::string _cardFaceRes[CFT_NUM_CARD_FACE_TYPES] = {
    "A.png",  // CFT_ACE
    "2.png",  // CFT_TWO
    "3.png",  // CFT_THREE
    "4.png",  // CFT_FOUR
    "5.png",  // CFT_FIVE
    "6.png",  // CFT_SIX
    "7.png",  // CFT_SEVEN
    "8.png",  // CFT_EIGHT
    "9.png",  // CFT_NINE
    "10.png", // CFT_TEN
    "J.png",  // CFT_JACK
    "Q.png",  // CFT_QUEEN
    "K.png",  // CFT_KING
};

class CardResConfig
{
public:
    // Get the resource name for a given card face and suit
    static void getCardRes(CardFaceType face, CardSuitType suit, std::string& suitRes, std::string& centerRes, std::string& cornerRes)
    {   
        if (face < CFT_NONE || face >= CFT_NUM_CARD_FACE_TYPES ||
            suit < CST_NONE || suit >= CST_NUM_CARD_SUIT_TYPES)
        {
            CCLOGERROR("Invalid card face or suit type.\n");
            return;
        }

        suitRes = "suits/" + _cardSuitRes[suit];

        std::string baseRes = _cardFaceRes[face];
        if (suit == CST_DIAMONDS || suit == CST_HEARTS)
        {
            centerRes = "number/big_red_" + baseRes;
            cornerRes = "number/small_red_" + baseRes;
        }
        else if (suit == CST_CLUBS || suit == CST_SPADES)
        {
            centerRes = "number/big_black_" + baseRes;
            cornerRes = "number/small_black_" + baseRes;
        }

        return;
    }
};

#endif // CARD_RES_CONFIG_H