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
  auto entities = gameManager->getEntitiesForCurrentScene();
  for (size_t i = 0; i < entities.size(); ++i) {
    auto* entity = entities[i];
    switch (entity->getType()) {
      case Entity::EntityType::ENTITY:
      case Entity::EntityType::SPRITE:
        entity->render(state->renderer);
        break;
      case Entity::EntityType::BUTTON:
        dynamic_cast<ButtonEntity*>(entity)->render(state->renderer, font);
        break;
      case Entity::EntityType::TEXT:
        dynamic_cast<TextEntity*>(entity)->render(state->renderer, font);
        break;
      case Entity::EntityType::CARD:
        dynamic_cast<CardEntity*>(entity)->render(state->renderer, i,
                                                  entities.size());
        break;
      default:
        continue;  // Skip unknown entity types
    }
  }

  SDL_RenderPresent(state->renderer);
  return SDL_APP_CONTINUE;
}