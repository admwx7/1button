#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <string>

#include "Entity.h"

class TextEntity : public Entity {
 private:
  std::string text;
  TTF_Font* font = nullptr;

 public:
  TextEntity(SDL_Texture* texture, const SDL_FRect& pos,
             const std::string& text, TTF_Font* font)
      : Entity(texture, pos), text(text), font(font) {}
};