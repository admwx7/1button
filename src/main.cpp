#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <iostream>

#include "../include/GameManager.h"
#include "../include/GameState.h"
#include "../include/ScreenManager.h"
#include "../include/TextureManager.h"

GameState* gameState = new GameState();
TextureManager* textureManager = nullptr;
GameManager* gameManager = nullptr;
ScreenManager* screenManager = nullptr;

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
  SDL_SetAppMetadata("1Button", "1.0.0", "com.example.1button");
  SDL_SetHint(SDL_HINT_GPU_DRIVER, "gpu");

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  // TODO: set virtual window size, maintain aspect ratio
  Uint32 windowFlags = SDL_WINDOW_RESIZABLE;
  if (!SDL_CreateWindowAndRenderer("1Button", gameState->windowDimensions.width,
                                   gameState->windowDimensions.height,
                                   windowFlags, &gameState->window,
                                   &gameState->renderer)) {
    SDL_Log("Failed to create window and renderer: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }
  if (TTF_Init() == false) {
    SDL_Log("Failed to initialize TTF: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  SDL_SetRenderLogicalPresentation(
      gameState->renderer, gameState->windowDimensions.width,
      gameState->windowDimensions.height, SDL_LOGICAL_PRESENTATION_LETTERBOX);

  textureManager = new TextureManager(gameState);
  gameManager = new GameManager(gameState, textureManager);
  screenManager = new ScreenManager(gameState, gameManager);
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
  if (event->type == SDL_EVENT_QUIT) {
    return SDL_APP_SUCCESS;
  }
  return gameManager->handleEvent(event);
}

SDL_AppResult SDL_AppIterate(void* appstate) {
  return screenManager->renderFrame();
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
  delete gameState;
  delete textureManager;
  delete gameManager;
  delete screenManager;
  TTF_Quit();
  SDL_Quit();
}