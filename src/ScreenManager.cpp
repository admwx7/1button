#include "../include/ScreenManager.h"

ScreenManager::ScreenManager(GameState* state, GameManager* gameManager)
    : state(state), gameManager(gameManager) {
  char* font_path = NULL;
  SDL_asprintf(&font_path, "%s../assets/%s", SDL_GetBasePath(), "cinzel.ttf");
  font = TTF_OpenFont(font_path, 42);
  TTF_SetFontStyle(font, TTF_STYLE_BOLD);
}
ScreenManager::~ScreenManager() {
  if (font) {
    TTF_CloseFont(font);
  }
}

SDL_AppResult ScreenManager::renderFrame() {
  SDL_SetRenderDrawColor(state->renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(state->renderer);

  const SceneManager::Scene scene = gameManager->getCurrentScene();
  if (!state->renderer || !font) {
    return SDL_APP_FAILURE;
  }
  for (auto* entity : gameManager->getEntitiesForCurrentScene()) {
    if (ButtonEntity* d_ptr = dynamic_cast<ButtonEntity*>(entity)) {
      d_ptr->render(state->renderer, font);
    } else {
      entity->render(state->renderer);
    }
  }

  SDL_RenderPresent(state->renderer);
  return SDL_APP_CONTINUE;
}