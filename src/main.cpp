#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>

// TODO: transition to virtual resolution for better scaling
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 640

struct GameState {
  SDL_Window* window = nullptr;
  SDL_Renderer* renderer = nullptr;

  // TODO
  SDL_AppResult handleEvent(SDL_Event* event) { return SDL_APP_CONTINUE; }
  SDL_AppResult renderFrame() { return SDL_APP_CONTINUE; }
};
GameState* gameState = new GameState();

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
  // TODO: using unique_ptr for automatic cleanup
  SDL_SetAppMetadata("CatCatch", "1.0.0", "com.example.catcatch");

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  if (!SDL_CreateWindowAndRenderer("CatCatch", WINDOW_WIDTH, WINDOW_HEIGHT,
                                   SDL_WINDOW_VULKAN, &gameState->window,
                                   &gameState->renderer)) {
    SDL_Log("Failed to create window and renderer: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }
  if (TTF_Init()) {
    SDL_Log("Failed to initialize TTF");
    return SDL_APP_FAILURE;
  }

  // TODO: virtual resolution setup
  SDL_SetRenderLogicalPresentation(gameState->renderer, WINDOW_WIDTH,
                                   WINDOW_HEIGHT,
                                   SDL_LOGICAL_PRESENTATION_LETTERBOX);

  // TODO: Initialize game renderer and state
  // gameRenderer = new GameRenderer(renderer);
  // gameState = new GameState(gameRenderer);
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
  TTF_Quit();
  SDL_Quit();
}