#include "../configs/loaders/LevelConfigLoader.h"
#include "../../cocos2d/external/json/document.h"  // For JSON parsing

bool LevelConfigLoader::loadLevelConfig(const std::string& filePath, LevelConfig& config) {
    // Read the JSON file
    std::string fullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename(filePath);
    if (fullPath.empty()) {
        CCLOG("Failed to load file: %s", filePath.c_str());
        return false;
    }

    // Parse the JSON file
    rapidjson::Document doc;
    std::string fileContents = cocos2d::FileUtils::getInstance()->getStringFromFile(fullPath);
    doc.Parse(fileContents.c_str());

    if (doc.HasParseError()) {
        CCLOG("Error parsing JSON file: %s", filePath.c_str());
        return false;
    }

    //  Extract playfield data from JSON and populate the LevelConfig object
    if (doc.HasMember("Playfield") && doc["Playfield"].IsArray()) {
        const rapidjson::Value& playfield = doc["Playfield"];
        for (rapidjson::SizeType i = 0; i < playfield.Size(); ++i) {
            const rapidjson::Value& card = playfield[i];

            CardConfig cardConfig;
            // Parse CardFaceType
            cardConfig.cardFace = static_cast<CardFaceType>(card["CardFace"].GetInt());
            // Parse CardSuitType
            cardConfig.cardSuit = static_cast<CardSuitType>(card["CardSuit"].GetInt());
            // Parse Position
            cardConfig.position.x = card["Position"]["x"].GetInt();
            cardConfig.position.y = card["Position"]["y"].GetInt();

            // Add the card to the playfield
            config.addCardToPlayfield(cardConfig);
        }
    }

    // Extract stack data from JSON and populate the LevelConfig object
    if (doc.HasMember("Stack") && doc["Stack"].IsArray()) {
        const rapidjson::Value& stack = doc["Stack"];
        for (rapidjson::SizeType i = 0; i < stack.Size(); ++i) {
            const rapidjson::Value& card = stack[i];

            CardConfig cardConfig;
            // Parse CardFaceType
            cardConfig.cardFace = static_cast<CardFaceType>(card["CardFace"].GetInt());
            // Parse CardSuitType
            cardConfig.cardSuit = static_cast<CardSuitType>(card["CardSuit"].GetInt());
            // Parse Position
            cardConfig.position.x = card["Position"]["x"].GetInt();
            cardConfig.position.y = card["Position"]["y"].GetInt();

            // Add the card to stack
            config.addCardToStack(cardConfig);
        }
    }

    return true;
}
