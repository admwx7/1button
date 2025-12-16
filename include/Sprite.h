#pragma once
#include <SDL3/SDL.h>

#include <map>
#include <vector>

#include "Entity.h"

template <typename T>
class Sprite : public Entity {
  using AnimationMap = std::map<T, std::vector<SDL_FRect>>;
  AnimationMap animationMap = {};
  int frameRate = 100;  // milliseconds per frame
  int currentFrame = 0;

 public:
  Sprite(AnimationMap animMap, int frameRate)
      : animationMap(animMap), frameRate(frameRate) {};
};