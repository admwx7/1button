#include "../include/GameManager.h"

GameManager::GameManager(GameState* state, TextureManager* textureManager)
    : state(state), textureManager(textureManager) {
  sceneManager = new SceneManager(state, textureManager);
}
GameManager::~GameManager() {
  delete sceneManager;
  sceneManager = nullptr;
  selectedCards.clear();
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
      sceneManager->space_icon->setState(
          SceneManager::SpaceAnimationStates::IDLE);
      keyHoldStart = 0;
      return SDL_APP_CONTINUE;
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
    case SDL_EVENT_KEY_DOWN:
      if (keyHoldStart < 0) {
        // do nothing
      } else if (keyHoldStart == 0) {
        keyHoldStart = SDL_GetTicks();
        sceneManager->space_icon->setState(
            SceneManager::SpaceAnimationStates::ACTIVE);
      } else if (keyHeldDuration > KEY_HOLD_THRESHOLD) {
        longPressAction();
        keyHoldStart = -1;
        sceneManager->space_icon->setState(
            SceneManager::SpaceAnimationStates::IDLE);
      }
      return SDL_APP_CONTINUE;
    default:
      break;
  }
  return SDL_APP_CONTINUE;
}
void GameManager::longPressAction() {
  switch (getCurrentScene()) {
    // TODO: improve the association between index and action, now stored in
    // targetEntity
    case SceneManager::MAIN_MENU:
      switch (selectedOptionIndex) {
        case 0:
          sceneManager->changeScene(SceneManager::Scene::NEW_GAME);
          selectedCards.clear();
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
    case SceneManager::NEW_GAME:
      selectCard(SceneManager::NEW_CYCLE);
      break;
    case SceneManager::NEW_CYCLE:
      selectCard(SceneManager::DAY_CYCLE);
      break;
    case SceneManager::DAY_CYCLE:
      // TODO: store the day case, increment when hitting this state
      selectCard(SceneManager::NIGHT_CYCLE);
      break;
    case SceneManager::NIGHT_CYCLE:
      // TODO: check day counter against EVENT_TIMED triggers
      // TODO: check day counter against EVENT_RNG triggers

      // DAY or END_CYCLE
      break;
    case SceneManager::END_CYCLE:
      // CYCLE_UNLOCK or END_GAME
      break;
    case SceneManager::CYCLE_UNLOCK:
      selectCard(SceneManager::NEW_CYCLE);
      break;
    case SceneManager::END_GAME:
      sceneManager->changeScene(SceneManager::MAIN_MENU);
      break;
    case SceneManager::EVENT_RNG:
      // TODO: set when next EVENT_RNG will occur
      selectCard(SceneManager::NIGHT_CYCLE);
      break;
    case SceneManager::EVENT_TIMED:
      // TODO: set when next EVENT_TIMED will occur
      selectCard(SceneManager::NIGHT_CYCLE);
      break;
    default:
      break;
  }
  selectedOptionIndex = 0;
}
void GameManager::selectCard(SceneManager::Scene nextScene) {
  selectedCards.push_back(dynamic_cast<CardEntity*>(targetEntity));
  targetEntity = nullptr;
  sceneManager->changeScene(nextScene);
}
void GameManager::shortPressAction() {
  std::vector<Entity*> items;
  switch (getCurrentScene()) {
    case SceneManager::MAIN_MENU: {
      items = sceneManager->getEntitiesForSceneComponent(
          SceneManager::MAIN_MENU_BUTTONS);
      break;
    }
    case SceneManager::NEW_GAME:
      items = sceneManager->getEntitiesForSceneComponent(
          SceneManager::CARDS_GAME_MODIFIERS);
      break;
    case SceneManager::EVENT_RNG:
      items = sceneManager->getEntitiesForSceneComponent(
          SceneManager::CARDS_EVENTS_RNG);
      break;
    case SceneManager::EVENT_TIMED:
      items = sceneManager->getEntitiesForSceneComponent(
          SceneManager::CARDS_EVENTS_TIMED);
      break;
    case SceneManager::DAY_CYCLE:
      items = sceneManager->getEntitiesForSceneComponent(
          SceneManager::CARDS_CYCLE_DAY);
      break;
    case SceneManager::END_CYCLE:
      items = sceneManager->getEntitiesForSceneComponent(
          SceneManager::CARDS_CYCLE_END);
      break;
    case SceneManager::NEW_CYCLE:
      items = sceneManager->getEntitiesForSceneComponent(
          SceneManager::CARDS_CYCLE_MODIFIERS);
      break;
    // TDOO: handle other scenes
    case SceneManager::NIGHT_CYCLE:
    case SceneManager::END_GAME:
    case SceneManager::CYCLE_SUMMARY:
    case SceneManager::GAME_SUMMARY:
    case SceneManager::SETTINGS_MENU:
      // TODO: handle settings at some point
      break;
    default:
      SDL_Log("Short press action in other scene");
      break;
  }
  if (ButtonEntity* d_ptr =
          dynamic_cast<ButtonEntity*>(items.at(selectedOptionIndex))) {
    d_ptr->setState(ButtonEntity::IDLE);
  } else if (Sprite<CardEntity::CardState>* c_ptr =
                 dynamic_cast<Sprite<CardEntity::CardState>*>(
                     items.at(selectedOptionIndex))) {
    c_ptr->setState(CardEntity::CardState::SELECT);
  }
  selectedOptionIndex = (selectedOptionIndex + 1) % items.size();
  targetEntity = items.at(selectedOptionIndex);
  if (ButtonEntity* d_ptr = dynamic_cast<ButtonEntity*>(targetEntity)) {
    d_ptr->setState(ButtonEntity::SELECTED);
  } else if (Sprite<CardEntity::CardState>* c_ptr =
                 dynamic_cast<Sprite<CardEntity::CardState>*>(targetEntity)) {
    c_ptr->setState(CardEntity::CardState::SELECT);
  }
}