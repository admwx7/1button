#pragma once
#include <SDL3/SDL.h>

struct WindowDimensions {
  int width = 640;
  int height = 360;
};

class GameState {
 public:
  inline static WindowDimensions windowDimensions;
  SDL_Window* window = nullptr;
  SDL_Renderer* renderer = nullptr;

  ~GameState() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
  }
};