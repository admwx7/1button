#include "../include/CardManager.h"

CardEntity* CardManager::createCardEntity(
    TextureManager::TextureName textureName) {
  return new CardEntity(
      textureManager->getTexture(textureName),
      [textureName](void* _gameState, void* _prevCard) {
        GameState* gameState = static_cast<GameState*>(_gameState);
        CardEntity* prevCard = static_cast<CardEntity*>(_prevCard);

        switch (textureName) {
          case TextureManager::TextureName::CARD_PLUS_1:
            gameState->addScore(1);
            break;
          case TextureManager::TextureName::CARD_PLUS_2:
            gameState->addScore(2);
            break;
          case TextureManager::TextureName::CARD_PLUS_3:
            gameState->addScore(3);
            break;
          case TextureManager::TextureName::CARD_TIMES_2:
            gameState->setScore(gameState->getScore() * 2);
            break;
          case TextureManager::TextureName::CARD_TIMES_3:
            gameState->setScore(gameState->getScore() * 3);
            break;
          case TextureManager::TextureName::CARD_ECHO:
            if (prevCard != nullptr && prevCard->mutator != nullptr) {
              prevCard->mutator(gameState, nullptr);
            }
            break;
          case TextureManager::TextureName::CARD_WORM:
            gameState->setTarget((int)(gameState->getTarget() * 0.8f));
            break;
          default:
            break;
        }
      });
}

CardManager::CardManager(TextureManager* textureManager)
    : textureManager(textureManager) {
  initialize();
}
CardManager::~CardManager() { clearDecks(); }

void CardManager::initialize() {
  initializeDeck(DeckType::AVAILABLE);
  initializeDeck(DeckType::STACK);
  initializeDeck(DeckType::DISCARD);
}
void CardManager::clearDecks() {
  for (auto& [type, deck] : decks) {
    clearDeck(type);
  }
  decks.clear();
}
void CardManager::clearDeck(DeckType type) {
  auto& deck = decks[type];
  if (type != DeckType::STACK) {
    for (CardEntity* card : deck) {
      delete card;
    }
  }
  deck.clear();
}
void CardManager::initializeDeck(DeckType type) {
  switch (type) {
    case DeckType::AVAILABLE: {
      decks[type] = std::vector<CardEntity*>{
          createCardEntity(TextureManager::TextureName::CARD_PLUS_1),
          createCardEntity(TextureManager::TextureName::CARD_PLUS_1),
          createCardEntity(TextureManager::TextureName::CARD_PLUS_1),
          createCardEntity(TextureManager::TextureName::CARD_PLUS_1),
          createCardEntity(TextureManager::TextureName::CARD_PLUS_1),
          createCardEntity(TextureManager::TextureName::CARD_PLUS_1),
          createCardEntity(TextureManager::TextureName::CARD_PLUS_1),
          createCardEntity(TextureManager::TextureName::CARD_PLUS_2),
          createCardEntity(TextureManager::TextureName::CARD_PLUS_2),
          createCardEntity(TextureManager::TextureName::CARD_PLUS_2),
          createCardEntity(TextureManager::TextureName::CARD_PLUS_2),
          createCardEntity(TextureManager::TextureName::CARD_PLUS_3),
          createCardEntity(TextureManager::TextureName::CARD_TIMES_2),
          createCardEntity(TextureManager::TextureName::CARD_TIMES_2),
          createCardEntity(TextureManager::TextureName::CARD_TIMES_3),
          createCardEntity(TextureManager::TextureName::CARD_ECHO),
          createCardEntity(TextureManager::TextureName::CARD_WORM),
      };
      shuffleDeck(type);
      break;
    }
    case DeckType::STACK: {
      decks[type] = std::vector<CardEntity*>{};
      break;
    }
    case DeckType::DISCARD: {
      decks[type] = std::vector<CardEntity*>{};
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
std::vector<CardEntity*> CardManager::drawCards(DeckType type) {
  std::vector<CardEntity*> drawnCards = {};
  auto& deck = decks[type];
  int index = 0;
  for (int i = 0; i < DRAW_COUNT && deck.size() > 0; ++i) {
    drawnCards.push_back(deck.back());
    decks[DeckType::DISCARD].push_back(deck.back());
    deck.pop_back();
  }
  return drawnCards;
}
void CardManager::resetDeck(DeckType type) {
  auto& deck = decks[type];
  clearDeck(type);
  initializeDeck(type);
}
void CardManager::resetAllDecks() {
  for (auto& [type, deck] : decks) {
    resetDeck(type);
  }
}
void CardManager::addToStack(CardEntity* card) {
  decks[DeckType::STACK].push_back(card);
}
void CardManager::resolveStack(GameState* gameState) {
  auto& stackDeck = decks[DeckType::STACK];
  CardEntity* prevCard = nullptr;
  for (CardEntity* card : stackDeck) {
    if (card->mutator != nullptr) {
      card->mutator(gameState, prevCard);
    }
    prevCard = card;
  }
  prevCard = nullptr;
  stackDeck.clear();
}
bool CardManager::hasCards() const {
  return decks.at(DeckType::AVAILABLE).size() > DRAW_COUNT;
}