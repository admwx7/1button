#include "../include/GameManager.h"

GameManager::GameManager(GameState* state, TextureManager* textureManager)
    : state(state), textureManager(textureManager) {
  sceneManager = new SceneManager(state, textureManager);
}
GameManager::~GameManager() {
  delete sceneManager;
  sceneManager = nullptr;
}
SceneManager::Scene GameManager::getCurrentScene() const {
  return sceneManager->getCurrentScene();
}
std::vector<Entity*> GameManager::getEntitiesForCurrentScene() const {
  return sceneManager->getEntitiesForCurrentScene();
}
SDL_AppResult GameManager::handleEvent(SDL_Event* event) {
  uint64_t keyHeldDuration = SDL_GetTicks() - keyHoldStart;
  switch (event->type) {
    case SDL_EVENT_QUIT:
      return SDL_APP_SUCCESS;
    case SDL_EVENT_MOUSE_BUTTON_UP:
    case SDL_EVENT_KEY_UP:
      if (keyHoldStart <= 0) {
        // do nothing
      } else if (keyHeldDuration > KEY_HOLD_THRESHOLD) {
        longPressAction();
      } else {
        shortPressAction();
      }
      keyHoldStart = 0;
      return SDL_APP_CONTINUE;
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
    case SDL_EVENT_KEY_DOWN:
      if (keyHoldStart < 0) {
        // do nothing
      } else if (keyHoldStart == 0) {
        keyHoldStart = SDL_GetTicks();
      } else if (keyHeldDuration > KEY_HOLD_THRESHOLD) {
        longPressAction();
        keyHoldStart = -1;
      }
      return SDL_APP_CONTINUE;
    default:
      break;
  }
  return SDL_APP_CONTINUE;
}
void GameManager::longPressAction() {
  // TODO: improve the association between index and action
  switch (getCurrentScene()) {
    case SceneManager::MAIN_MENU:
      switch (selectedOptionIndex) {
        case 0:
          sceneManager->changeScene(SceneManager::Scene::NEW_GAME);
          break;
        case 1:
          sceneManager->changeScene(SceneManager::Scene::SETTINGS_MENU);
          break;
        case 2:
          SDL_Quit();
          exit(SDL_APP_SUCCESS);
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
  selectedOptionIndex = 0;
}
void GameManager::shortPressAction() {
  switch (getCurrentScene()) {
    case SceneManager::MAIN_MENU: {
      auto items = sceneManager->getEntitiesForSceneComponent(
          SceneManager::MAIN_MENU_BUTTONS);
      if (ButtonEntity* d_ptr =
              dynamic_cast<ButtonEntity*>(items.at(selectedOptionIndex))) {
        d_ptr->setState(ButtonEntity::IDLE);
      }
      selectedOptionIndex = (selectedOptionIndex + 1) % items.size();
      if (ButtonEntity* d_ptr =
              dynamic_cast<ButtonEntity*>(items.at(selectedOptionIndex))) {
        d_ptr->setState(ButtonEntity::SELECTED);
      }
      break;
    }
    default:
      SDL_Log("Short press action in other scene");
      break;
  }
}