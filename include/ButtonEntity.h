#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "Sprite.h"
#include "TextEntity.h"

class ButtonEntity : public TextEntity {
 public:
  enum ButtonState { IDLE, SELECTED };

 private:
  using ButtonTextureMap = std::unordered_map<ButtonState, SDL_FRect*>;
  ButtonTextureMap textureMap;
  ButtonState currentState = IDLE;

 public:
  ButtonEntity(ButtonState state, ButtonTextureMap textureMap,
               SDL_Texture* texture, SDL_FRect* position,
               const std::string& text,
               std::pair<float, float> offset = {0.0f, 0.0f})
      : TextEntity(texture, textureMap[state], position, text, offset),
        textureMap(textureMap),
        currentState(state) {}
  ~ButtonEntity() {
    for (auto& [state, rect] : textureMap) {
      delete rect;
    }
    textureMap.clear();
  }
  bool virtual render(SDL_Renderer* renderer, TTF_Font* font) {
    src = textureMap.at(currentState);
    return TextEntity::render(renderer, font);
  }
  void setState(ButtonState state) { currentState = state; }
};