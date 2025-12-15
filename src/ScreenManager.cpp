#include "../include/ScreenManager.h"

SDL_AppResult ScreenManager::renderFrame() {
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);
  return SDL_APP_CONTINUE;
}