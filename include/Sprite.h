#pragma once
#include <SDL3/SDL.h>

#include <iostream>
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
  bool loop = true;
  SDL_TimerID timerID = 0;

 private:
  static Uint32 callback(void* userdata, SDL_TimerID timerID, Uint32 interval) {
    Sprite* sprite = static_cast<Sprite<T>*>(userdata);
    return sprite->updateFrame() * interval;
  }

 public:
  Sprite(SDL_Texture* tex, SDL_FRect* pos, AnimationMap animMap, T initialState,
         int frameRate = 300, bool loop = true)
      : Entity(tex, NULL, pos),
        animationMap(animMap),
        currentState(initialState),
        frameRate(frameRate),
        loop(loop) {};
  ~Sprite() {
    for (auto& [key, frames] : animationMap) {
      for (auto& frame : frames) {
        delete frame;
        frame = nullptr;
      }
    }
    animationMap.clear();
    SDL_RemoveTimer(timerID);
  }
  virtual EntityType getType() const override { return EntityType::SPRITE; }
  virtual bool render(SDL_Renderer* renderer) const {
    if (texture == nullptr) return true;
    if (!visible) return false;
    auto frames = animationMap.at(currentState);

    return SDL_RenderTexture(renderer, texture, frames.at(currentFrame),
                             position);
  }
  void setFrameRate(int rate) { frameRate = rate; }
  void setState(T newState) {
    SDL_RemoveTimer(timerID);
    currentFrame = 0;
    currentState = newState;
    if (animationMap.at(currentState).size() > 0) {
      timerID = SDL_AddTimer(frameRate, callback, this);
    }
  }
  int updateFrame() {
    auto frames = animationMap.at(currentState);
    if (!loop && currentFrame >= frames.size() - 1) {
      return 0;  // Stop the timer if not looping and at last frame.
    } else if (currentFrame < frames.size() - 1) {
      currentFrame++;
    } else {
      currentFrame = 0;
    }
    return 1;
  }
};