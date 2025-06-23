#ifndef CARD_MODEL_H
#define CARD_MODEL_H

#include "../configs/models/CardResConfig.h"
#include <unordered_set>

class CardModel
{
private:
    unsigned int _uid;                               // Unique identifier for the card
    std::unordered_set<unsigned int> _coveredByIds;  // IDs of cards that cover this card
    std::unordered_set<unsigned int> _coverIds;      // IDs of cards that this card covers
    bool _isInteractable = true;                     // Whether the card can be interacted with
    const CardFaceType _kFace;                       // The face of the card
    const CardSuitType _kSuit;                       // The suit of the card
    CardPosition _position;                          // The position of the card on the screen
    cocos2d::Rect _boundingBox;                      // Bounding box for the card, used for collision detection   

public:
    CardModel(const unsigned int Uuid, const CardConfig& cardConfig)
        : _uid(Uuid), _kFace(cardConfig.cardFace),
        _kSuit(cardConfig.cardSuit),
        _position(cardConfig.position.x, cardConfig.position.y) {
    }

    // Getter methods
    unsigned int getUid() const { return _uid; }
    CardFaceType getFace() const { return _kFace; }
    CardSuitType getSuit() const { return _kSuit; }
    CardPosition getPosition() const { return _position; }
    bool getInteractable() const { return _isInteractable; }
    const std::unordered_set<unsigned int>& getCoveredByIds() const { return _coveredByIds; }
    const std::unordered_set<unsigned int>& getCoverIds() const { return _coverIds; }
    cocos2d::Rect getBoundingBox() const { return _boundingBox; }

    void flip() { _isInteractable = !_isInteractable; }  // Flips the card (face up or face down)
    void moveTo(const CardPosition& newPos) { _position = newPos; }  // Moves the card to a new position

    void setBoundingBox(const cocos2d::Rect& boundingBox) { _boundingBox = boundingBox; } // Sets the bounding box for the card
    void setInteractable(bool interactable) { _isInteractable = interactable; } // Sets whether the card can be interacted with
    void setPosition(int x, int y) { _position.x = x; _position.y = y; } // Sets the position of the card

    void addCoveredBy(int otherId) { // Adds an ID of a card that covers this card
        _coveredByIds.insert(otherId);
        _isInteractable = false;
    }
    void removeCoveredBy(int otherId) { // Removes an ID of a card that covers this card and checks if it can be interacted with
        _coveredByIds.erase(otherId);
        if (_coveredByIds.empty()) {
            _isInteractable = true;
        }
    }
    void addCover(int otherID) { // Adds an ID of a card that this card covers
        _coverIds.insert(otherID);
	}
};

#endif // CARD_MODEL_H