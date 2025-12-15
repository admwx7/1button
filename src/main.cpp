#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "../include/GameManager.h"
#include "../include/ScreenManager.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

struct GameState {
  SDL_Window* window = nullptr;
  SDL_Renderer* renderer = nullptr;
  GameManager* gameManager = nullptr;
  ScreenManager* screenManager = nullptr;

  ~GameState() {
    delete gameManager;
    delete screenManager;
  }

  SDL_AppResult handleEvent(SDL_Event* event) {
    return gameManager->handleEvent(event);
  }
  SDL_AppResult renderFrame() { return screenManager->renderFrame(); }
};
GameState* gameState = new GameState();

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
  SDL_SetAppMetadata("1Button", "1.0.0", "com.example.1button");

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  // TODO: set virtual window size, maintain aspect ratio
  Uint32 windowFlags = SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE;
  if (!SDL_CreateWindowAndRenderer("1Button", WINDOW_WIDTH, WINDOW_HEIGHT,
                                   windowFlags, &gameState->window,
                                   &gameState->renderer)) {
    SDL_Log("Failed to create window and renderer: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }
  if (TTF_Init() == false) {
    SDL_Log("Failed to initialize TTF: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  SDL_SetRenderLogicalPresentation(gameState->renderer, WINDOW_WIDTH,
                                   WINDOW_HEIGHT,
                                   SDL_LOGICAL_PRESENTATION_LETTERBOX);

  gameState->gameManager = new GameManager();
  gameState->screenManager =
      new ScreenManager(gameState->renderer, gameState->gameManager);
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
  if (event->type == SDL_EVENT_QUIT) {
    return SDL_APP_SUCCESS;
  }
  return gameState->handleEvent(event);
}

SDL_AppResult SDL_AppIterate(void* appstate) {
  return gameState->renderFrame();
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
  SDL_DestroyRenderer(gameState->renderer);
  SDL_DestroyWindow(gameState->window);
  delete gameState;
  TTF_Quit();
  SDL_Quit();
}