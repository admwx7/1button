#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <iostream>

#include "../include/GameManager.h"
#include "../include/GlobalState.h"
#include "../include/ScreenManager.h"
#include "../include/TextureManager.h"

GlobalState* globalState = new GlobalState();
TextureManager* textureManager = nullptr;
GameManager* gameManager = nullptr;
ScreenManager* screenManager = nullptr;

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
  SDL_SetAppMetadata("1Button", "1.0.0", "com.example.1button");
  SDL_SetHint(SDL_HINT_GPU_DRIVER, "gpu");

  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
    SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  // TODO: set virtual window size, maintain aspect ratio
  Uint32 windowFlags = SDL_WINDOW_RESIZABLE;
  if (!SDL_CreateWindowAndRenderer(
          "1Button", globalState->windowDimensions.width,
          globalState->windowDimensions.height, windowFlags,
          &globalState->window, &globalState->renderer)) {
    SDL_Log("Failed to create window and renderer: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }
  if (TTF_Init() == false) {
    SDL_Log("Failed to initialize TTF: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  SDL_SetRenderLogicalPresentation(
      globalState->renderer, globalState->windowDimensions.width,
      globalState->windowDimensions.height, SDL_LOGICAL_PRESENTATION_LETTERBOX);

  textureManager = new TextureManager(globalState);
  gameManager = new GameManager(globalState, textureManager);
  screenManager = new ScreenManager(globalState, gameManager);
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
  delete globalState;
  globalState = nullptr;
  delete textureManager;
  textureManager = nullptr;
  delete gameManager;
  gameManager = nullptr;
  delete screenManager;
  screenManager = nullptr;
  TTF_Quit();
  SDL_Quit();
}