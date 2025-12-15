#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "GameManager.h"

class ScreenManager {
 private:
  SDL_Renderer* renderer = nullptr;
  GameManager* gameManager = nullptr;
  TTF_Font* font = nullptr;

 public:
  ScreenManager(SDL_Renderer* renderer, GameManager* gameManager)
      : renderer(renderer), gameManager(gameManager) {
    char* font_path = NULL;
    SDL_asprintf(&font_path, "%s../assets/%s", SDL_GetBasePath(), "cinzel.ttf");
    font = TTF_OpenFont(font_path, 42);
    TTF_SetFontStyle(font, TTF_STYLE_BOLD);
  }
  ~ScreenManager() {
    if (font) {
      TTF_CloseFont(font);
    }
  }
  SDL_AppResult renderFrame();
};