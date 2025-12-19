#pragma once
#include <algorithm>
#include <random>
#include <unordered_map>
#include <vector>

#include "CardEntity.h"
#include "TextureManager.h"

class CardManager {
 public:
  enum class DeckType {
    RUN_MODIFIERS,
    RUN_UNLOCKS,
    GAME_MODIFIERS,
    GAME_GENRES,
    GAME_MECHANICS,
    EVENTS_RNG,
    EVENTS_TIMED,  // This one will be weird since it's a set recurrence for
                   // each and not ranomdized...
    PLAYER_ACTIONS,
  };

 private:
  TextureManager* textureManager = nullptr;
  std::unordered_map<DeckType, std::vector<CardEntity*>> decks = {};

  void clearDecks();
  void clearDeck(DeckType type);

 public:
  CardManager(TextureManager* textureManager);
  ~CardManager();

  void initializeDeck(DeckType type);
  void shuffleDeck(DeckType type);
  std::vector<CardEntity*> drawCards(DeckType type, int count,
                                     bool returnUnique = false);
  void resetDeck(DeckType type);
  void resetAllDecks();
};