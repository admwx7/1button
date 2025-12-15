#pragma once
#include <SDL3/SDL.h>

#include <string>
#include <unordered_map>
#include <vector>

class GameManager {
 public:
  GameManager() {
    // Initialize game manager with renderer
  }

  ~GameManager() {
    // Cleanup resources
  }

  SDL_AppResult handleEvent(SDL_Event* event);
};