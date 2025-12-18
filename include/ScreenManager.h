#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "GameManager.h"
#include "GameState.h"
#include "SceneManager.h"
#include "TextureManager.h"

class ScreenManager {
 private:
  GameState* state = nullptr;
  GameManager* gameManager = nullptr;
  TTF_Font* font = nullptr;

 public:
  ScreenManager(GameState* state, GameManager* gameManager);
  ~ScreenManager();
  SDL_AppResult renderFrame();
};