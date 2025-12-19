#include "../include/CardManager.h"

CardManager::CardManager(TextureManager* textureManager)
    : textureManager(textureManager) {
  initializeDeck(DeckType::RUN_MODIFIERS);
  initializeDeck(DeckType::RUN_UNLOCKS);
  initializeDeck(DeckType::GAME_MODIFIERS);
  initializeDeck(DeckType::GAME_GENRES);
  initializeDeck(DeckType::GAME_MECHANICS);
  initializeDeck(DeckType::EVENTS_RNG);
  initializeDeck(DeckType::EVENTS_TIMED);
  initializeDeck(DeckType::PLAYER_ACTIONS);
}
CardManager::~CardManager() {
  for (auto& [type, deck] : decks) {
    for (CardEntity* card : deck) {
      delete card;
    }
    deck.clear();
  }
  decks.clear();
}

void CardManager::clearDecks() {
  for (auto& [type, deck] : decks) {
    clearDeck(type);
  }
  decks.clear();
}
void CardManager::clearDeck(DeckType type) {
  auto& deck = decks[type];
  for (CardEntity* card : deck) {
    delete card;
  }
  deck.clear();
}
void CardManager::initializeDeck(DeckType type) {
  switch (type) {
    case DeckType::RUN_MODIFIERS: {
      SDL_Log("The thing!");
      decks[type] = std::vector<CardEntity*>{
          new CardEntity(textureManager->getTexture(
                             TextureManager::TextureName::CARD_GAME_GENRE_RPG),
                         nullptr),
          new CardEntity(textureManager->getTexture(
                             TextureManager::TextureName::CARD_GAME_GENRE_RPG),
                         nullptr),
          new CardEntity(textureManager->getTexture(
                             TextureManager::TextureName::CARD_GAME_GENRE_RPG),
                         nullptr),
      };
      break;
    }
    case DeckType::RUN_UNLOCKS: {
      decks[type] = std::vector<CardEntity*>{
          new CardEntity(textureManager->getTexture(
                             TextureManager::TextureName::CARD_GAME_GENRE_RPG),
                         nullptr),
          new CardEntity(textureManager->getTexture(
                             TextureManager::TextureName::CARD_GAME_GENRE_RPG),
                         nullptr),
          new CardEntity(textureManager->getTexture(
                             TextureManager::TextureName::CARD_GAME_GENRE_RPG),
                         nullptr),
      };
      break;
    }
    case DeckType::GAME_MODIFIERS: {
      decks[type] = std::vector<CardEntity*>{
          new CardEntity(textureManager->getTexture(
                             TextureManager::TextureName::CARD_GAME_GENRE_RPG),
                         nullptr),
          new CardEntity(textureManager->getTexture(
                             TextureManager::TextureName::CARD_GAME_GENRE_RPG),
                         nullptr),
          new CardEntity(textureManager->getTexture(
                             TextureManager::TextureName::CARD_GAME_GENRE_RPG),
                         nullptr),
      };
      break;
    }
    case DeckType::GAME_GENRES: {
      decks[type] = std::vector<CardEntity*>{
          new CardEntity(
              textureManager->getTexture(
                  TextureManager::TextureName::CARD_GAME_GENRE_ACTION),
              nullptr),
          new CardEntity(textureManager->getTexture(
                             TextureManager::TextureName::CARD_GAME_GENRE_RPG),
                         nullptr),
      };
      break;
    }
    case DeckType::GAME_MECHANICS: {
      decks[type] = std::vector<CardEntity*>{
          new CardEntity(textureManager->getTexture(
                             TextureManager::TextureName::CARD_GAME_GENRE_RPG),
                         nullptr),
          new CardEntity(textureManager->getTexture(
                             TextureManager::TextureName::CARD_GAME_GENRE_RPG),
                         nullptr),
          new CardEntity(textureManager->getTexture(
                             TextureManager::TextureName::CARD_GAME_GENRE_RPG),
                         nullptr),
      };
      break;
    }
    case DeckType::EVENTS_RNG: {
      decks[type] = std::vector<CardEntity*>{
          new CardEntity(textureManager->getTexture(
                             TextureManager::TextureName::CARD_GAME_GENRE_RPG),
                         nullptr),
          new CardEntity(textureManager->getTexture(
                             TextureManager::TextureName::CARD_GAME_GENRE_RPG),
                         nullptr),
          new CardEntity(textureManager->getTexture(
                             TextureManager::TextureName::CARD_GAME_GENRE_RPG),
                         nullptr),
      };
      break;
    }
    case DeckType::EVENTS_TIMED: {
      decks[type] = std::vector<CardEntity*>{
          new CardEntity(textureManager->getTexture(
                             TextureManager::TextureName::CARD_GAME_GENRE_RPG),
                         nullptr),
          new CardEntity(textureManager->getTexture(
                             TextureManager::TextureName::CARD_GAME_GENRE_RPG),
                         nullptr),
          new CardEntity(textureManager->getTexture(
                             TextureManager::TextureName::CARD_GAME_GENRE_RPG),
                         nullptr),
      };
      break;
    }
    case DeckType::PLAYER_ACTIONS: {
      decks[type] = std::vector<CardEntity*>{
          new CardEntity(textureManager->getTexture(
                             TextureManager::TextureName::CARD_GAME_GENRE_RPG),
                         nullptr),
          new CardEntity(textureManager->getTexture(
                             TextureManager::TextureName::CARD_GAME_GENRE_RPG),
                         nullptr),
          new CardEntity(textureManager->getTexture(
                             TextureManager::TextureName::CARD_GAME_GENRE_RPG),
                         nullptr),
      };
      break;
    }
    default:
      break;
  }
}
void CardManager::shuffleDeck(DeckType type) {
  auto& deck = decks[type];
  std::shuffle(deck.begin(), deck.end(), std::mt19937{std::random_device{}()});
}
std::vector<CardEntity*> CardManager::drawCards(DeckType type, int count,
                                                bool returnUnique) {
  std::vector<CardEntity*> drawnCards;
  auto& deck = decks[type];
  int index = 0;
  for (int i = 0; i < count; ++i) {
    if (returnUnique && i < deck.size()) {
      index = i;
    } else if (returnUnique && i >= deck.size()) {
      break;
    } else {
      index = random() % deck.size();
    }
    drawnCards.push_back(deck.at(index));
  }
  return drawnCards;
}
void CardManager::resetDeck(DeckType type) {
  auto& deck = decks[type];
  for (CardEntity* card : deck) {
    delete card;
  }
  deck.clear();
  initializeDeck(type);
  shuffleDeck(type);
}
void CardManager::resetAllDecks() {
  for (auto& [type, deck] : decks) {
    resetDeck(type);
  }
}
