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
  enum ButtonAction { NEW_GAME, SETTINGS, EXIT_GAME };

 private:
  inline static const std::unordered_map<ButtonAction, std::string> textMap = {
      {NEW_GAME, "New Game"}, {SETTINGS, "Settings"}, {EXIT_GAME, "Exit Game"}};
  using ButtonTextureMap = std::unordered_map<ButtonState, SDL_FRect*>;
  ButtonTextureMap textureMap;
  ButtonState currentState = IDLE;
  ButtonAction action;

 public:
  ButtonEntity(ButtonState state, ButtonTextureMap textureMap,
               SDL_Texture* texture, SDL_FRect* position, ButtonAction action,
               std::pair<float, float> offset = {0.0f, 0.0f})
      : TextEntity(texture, textureMap[state], position, textMap.at(action),
                   offset),
        action(action),
        textureMap(textureMap),
        currentState(state) {}
  ~ButtonEntity() {
    for (auto& [state, rect] : textureMap) {
      delete rect;
    }
    textureMap.clear();
  }
  virtual Entity::EntityType getType() const override {
    return Entity::EntityType::BUTTON;
  }
  virtual bool render(SDL_Renderer* renderer, TTF_Font* font) {
    src = textureMap.at(currentState);
    return TextEntity::render(renderer, font);
  }
  void setState(ButtonState state) { currentState = state; }
  ButtonAction getAction() const { return action; }
};