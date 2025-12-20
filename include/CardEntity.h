#pragma once
#include <functional>
#include <memory>
#include <string>

#include "Entity.h"
#include "GlobalState.h"
#include "Sprite.h"

struct CardDimension {
  static constexpr float WIDTH = 112.0f;
  static constexpr float HEIGHT = 160.0f;
  static constexpr float GAP = 20.0f;
};

class CardEntity : public Entity {
 public:
  enum class CardState { IDLE, SELECTED };
  std::function<void(void* gameState, void* prevCard)> mutator = nullptr;

 private:
  Sprite<CardState>* sprite = nullptr;
  inline static CardDimension cardDimension;
  inline static const SDL_FRect position = {
      (GlobalState::windowDimensions.width / 2) - 56.0f,
      (GlobalState::windowDimensions.height / 2) - 80.0f, cardDimension.WIDTH,
      cardDimension.HEIGHT};

 public:
  CardEntity(SDL_Texture* texture,
             std::function<void(void* gameState, void* prevCard)> callback)
      : Entity(texture, NULL, NULL),
        sprite(new Sprite<CardState>(
            texture, new SDL_FRect{position},
            {{CardState::IDLE,
              {new SDL_FRect{0, 0, cardDimension.WIDTH, cardDimension.HEIGHT}}},
             {CardState::SELECTED,
              {new SDL_FRect{cardDimension.WIDTH, 0, cardDimension.WIDTH,
                             cardDimension.HEIGHT}}}},
            CardState::IDLE, 100, false)),
        mutator(callback) {};
  ~CardEntity() {
    if (sprite != nullptr) {
      delete sprite;
      sprite = nullptr;
    }
  }
  void setState(CardState newState) { sprite->setState(newState); }
  virtual EntityType getType() const override { return EntityType::CARD; }
  virtual bool render(SDL_Renderer* renderer) const override {
    return sprite->render(renderer);
  }
  virtual bool render(SDL_Renderer* renderer, int index, int count) const {
    float totalWidth =
        count * (cardDimension.WIDTH + cardDimension.GAP) - cardDimension.GAP;
    float startX = (GlobalState::windowDimensions.width - totalWidth) / 2.0f;
    startX += index * (cardDimension.WIDTH + cardDimension.GAP);
    sprite->setPosition(
        new SDL_FRect{startX, position.y, position.w, position.h});
    return sprite->render(renderer);
  }
};