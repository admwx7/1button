#pragma once
#include <SDL3/SDL.h>

#include <unordered_map>
#include <vector>

#include "Entity.h"

template <typename T>
class Sprite : public Entity {
  using AnimationMap = std::unordered_map<T, std::vector<SDL_FRect*>>;
  AnimationMap animationMap = {};
  int frameRate = 300;  // milliseconds per frame
  int currentFrame = 0;
  T currentState;

 public:
  Sprite(SDL_Texture* tex, SDL_FRect* pos, AnimationMap animMap, T initialState)
      : Entity(tex, NULL, pos),
        animationMap(animMap),
        currentState(initialState) {};
  ~Sprite() {
    for (auto& [key, frames] : animationMap) {
      for (auto& frame : frames) {
        delete frame;
        frame = nullptr;
      }
    }
    animationMap.clear();
  }
  bool virtual render(SDL_Renderer* renderer) const {
    auto frames = animationMap.at(currentState);
    src = frames.at(currentFrame);

    // TODO: add an flag for repeat or not
    // Uint32 ticks = SDL_GetTicks();
    // currentFrame = (ticks / frameRate) % frames.size();
    // src = frames[currentFrame];
    return Entity::render(renderer);
  }
  void setFrameRate(int rate) { frameRate = rate; }
};