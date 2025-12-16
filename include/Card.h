#pragma once
#include "Sprite.h"

class Card : public Sprite<Card::CardState> {
 public:
  enum class CardState { IDLE, SELECT };
  Card(std::vector<SDL_FRect> idleAnimations,
       std::vector<SDL_FRect> selectAnimations)
      : Sprite<Card::CardState>({{CardState::IDLE, idleAnimations},
                                 {CardState::SELECT, selectAnimations}},
                                100) {};
};