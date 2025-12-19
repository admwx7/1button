#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <algorithm>
#include <string>
#include <utility>

#include "Entity.h"

class TextEntity : public Entity {
 private:
  std::string text;
  SDL_Color color = {255, 255, 255, 255};
  std::pair<float, float> offset = {0.0f, 0.0f};

 public:
  TextEntity(SDL_Texture* tex, SDL_FRect* src, SDL_FRect* pos,
             const std::string& text,
             std::pair<float, float> offset = {0.0f, 0.0f})
      : Entity(tex, src, pos), text(text), offset(offset) {}
  virtual EntityType getType() const override { return EntityType::TEXT; }
  bool virtual render(SDL_Renderer* renderer, TTF_Font* font) const {
    if (!Entity::render(renderer)) {
      return false;
    }

    return renderText(renderer, font);
  }
  bool renderText(SDL_Renderer* renderer, TTF_Font* font) const {
    if (font && !text.empty()) {
      SDL_Surface* textSurface =
          TTF_RenderText_Blended(font, text.c_str(), text.size(), color);
      if (!textSurface) {
        return false;
      }

      SDL_Texture* textTexture =
          SDL_CreateTextureFromSurface(renderer, textSurface);
      SDL_DestroySurface(textSurface);
      if (!textTexture) {
        return false;
      }

      // Center logic
      SDL_FRect dst;
      float h_scale = 1.0f, w_scale = 1.0f;

      dst.x = position->x + offset.first;
      dst.y = position->y + offset.second;
      SDL_GetTextureSize(textTexture, &dst.w, &dst.h);
      w_scale = (position->w - offset.first * 2) / dst.w;
      h_scale = (position->h - offset.second * 2) / dst.h;
      float scale = std::min(h_scale, w_scale);
      dst.w *= scale;
      dst.h *= scale;
      dst.x = position->x + (position->w - dst.w) / 2;
      dst.y = position->y + (position->h - dst.h) / 2;

      bool result = SDL_RenderTexture(renderer, textTexture, NULL, &dst);
      SDL_DestroyTexture(textTexture);
      return result;
    }
    return true;
  }
};