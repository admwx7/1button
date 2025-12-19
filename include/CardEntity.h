#pragma once
#include <string>

#include "Sprite.h"

class CardEntity : Sprite<CardEntity::CardState> {
 public:
  enum class CardState { IDLE, SELECT };
  typedef Uint32(SDLCALL* CardMutatorCallback)(void* data);

 private:
  CardMutatorCallback mutator = nullptr;

  // TODO: add entries to the animationMap
 public:
  CardEntity(SDL_Texture* texture, CardMutatorCallback callback)
      : Sprite<CardEntity::CardState>(
            texture, new SDL_FRect{0, 0, 100, 150},
            {{CardState::IDLE, {new SDL_FRect{0, 0, 100, 150}}},
             {CardState::SELECT, {new SDL_FRect{0, 150, 100, 150}}}},
            CardState::IDLE, 100, false),
        mutator(callback) {};
  ~CardEntity() {
    if (position) {
      delete position;
      position = nullptr;
    }
  }
  virtual EntityType getType() const override { return EntityType::CARD; }
  virtual bool render(SDL_Renderer* renderer) const override {
    return Sprite<CardEntity::CardState>::render(renderer);
  }
  virtual bool render(SDL_Renderer* renderer, int index, int count) const {
    // TODO: use index to offset the card position
    return render(renderer);
  }
};