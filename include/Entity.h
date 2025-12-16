#pragma once
#include <SDL3/SDL.h>

class Entity {
 protected:
  SDL_Texture* texture = nullptr;
  SDL_FRect position = {0, 0, 0, 0};

  Entity(SDL_Texture* tex, const SDL_FRect& pos)
      : texture(tex), position(pos) {}
  ~Entity() {
    if (texture) {
      SDL_DestroyTexture(texture);
    }
  }
};