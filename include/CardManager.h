#pragma once
#include <algorithm>
#include <random>
#include <unordered_map>
#include <vector>

#include "CardEntity.h"
#include "GameState.h"
#include "TextureManager.h"

class CardManager {
 public:
  enum class DeckType {
    AVAILABLE,
    STACK,
    DISCARD,
  };

 private:
  int DRAW_COUNT = 3;
  TextureManager* textureManager = nullptr;
  std::unordered_map<DeckType, std::vector<CardEntity*>> decks = {};

  void clearDecks();
  void clearDeck(DeckType type);
  CardEntity* createCardEntity(TextureManager::TextureName textureName);

 public:
  CardManager(TextureManager* textureManager);
  ~CardManager();

  void initialize();
  void initializeDeck(DeckType type);
  void shuffleDeck(DeckType type);
  std::vector<CardEntity*> drawCards(DeckType type);
  void resetDeck(DeckType type);
  void resetAllDecks();
  void addToStack(CardEntity* card);
  void resolveStack(GameState* gameState);
  bool hasCards() const;
};