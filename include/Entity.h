#pragma once
#include <SDL3/SDL.h>

class Entity {
 protected:
  SDL_Texture* texture = nullptr;
  SDL_FRect* src = NULL;
  SDL_FRect* position = NULL;
  bool visible = true;

 public:
  enum EntityType {
    ENTITY,
    SPRITE,
    BUTTON,
    TEXT,
    CARD,
  };
  Entity(SDL_Texture* tex, SDL_FRect* src, SDL_FRect* pos)
      : texture(tex), src(src), position(pos) {}
  ~Entity() {
    if (src) {
      delete src;
      src = NULL;
    }
    if (position) {
      delete position;
      position = NULL;
    }
  }
  virtual EntityType getType() const { return EntityType::ENTITY; }
  bool virtual render(SDL_Renderer* renderer) const {
    if (texture == nullptr) return true;
    if (!visible) return false;
    return SDL_RenderTexture(renderer, texture, src, position);
  }
  void setVisible(bool vis) { visible = vis; }
  void setPosition(SDL_FRect* pos) {
    if (position) {
      delete position;
    }
    position = pos;
  }
};