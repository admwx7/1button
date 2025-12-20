#include "../include/GameManager.h"

void updateEntityState(std::vector<Entity*>::iterator& selectedOptionIterator,
                       CardEntity::CardState newState) {
  if (selectedOptionIterator != std::vector<Entity*>::iterator{}) {
    switch ((*selectedOptionIterator)->getType()) {
      case Entity::EntityType::BUTTON: {
        ButtonEntity* button =
            dynamic_cast<ButtonEntity*>(*selectedOptionIterator);
        button->setState((newState == CardEntity::CardState::SELECTED)
                             ? ButtonEntity::ButtonState::SELECTED
                             : ButtonEntity::ButtonState::IDLE);
        break;
      }
      case Entity::EntityType::CARD: {
        CardEntity* card = dynamic_cast<CardEntity*>(*selectedOptionIterator);
        card->setState(newState);
        break;
      }
      default:
        break;
    }
  }
}

GameManager::GameManager(GlobalState* state, TextureManager* textureManager)
    : state(state), textureManager(textureManager) {
  cardManager = new CardManager(textureManager);
  sceneManager =
      new SceneManager(state, textureManager, cardManager,
                       std::bind(&GameManager::sceneChangedCallback, this));
  sceneChangedCallback();
}
GameManager::~GameManager() {
  delete sceneManager;
  sceneManager = nullptr;
  delete cardManager;
  cardManager = nullptr;
}
void GameManager::sceneChangedCallback() {
  assert(sceneManager != nullptr);

  updateEntityState(selectedOptionIterator, CardEntity::CardState::IDLE);
  selectableEntities.clear();
  switch (sceneManager->getCurrentScene()) {
    case SceneManager::Scene::MAIN_MENU: {
      selectableEntities = sceneManager->getEntitiesForSceneComponent(
          SceneManager::MAIN_MENU_BUTTONS);
      break;
    }
    case SceneManager::Scene::NEW_RUN:
      selectableEntities = sceneManager->getEntitiesForSceneComponent(
          SceneManager::CARDS_RUN_MODIFIERS);
      break;
    case SceneManager::Scene::EVENT_RNG:
      selectableEntities = sceneManager->getEntitiesForSceneComponent(
          SceneManager::CARDS_EVENTS_RNG);
      break;
    case SceneManager::Scene::EVENT_TIMED:
      selectableEntities = sceneManager->getEntitiesForSceneComponent(
          SceneManager::CARDS_EVENTS_TIMED);
      break;
    case SceneManager::Scene::DAY_CYCLE:
      selectableEntities =
          sceneManager->getEntitiesForSceneComponent(SceneManager::CARDS_DAY);
      break;
    case SceneManager::Scene::END_CYCLE:
      selectableEntities = sceneManager->getEntitiesForSceneComponent(
          SceneManager::CARDS_UNLOCK);
      break;
    case SceneManager::Scene::NEW_GAME:
      selectableEntities = sceneManager->getEntitiesForSceneComponent(
          SceneManager::CARDS_GAME_MODIFIERS);
      break;
    // TODO: handle other scenes
    case SceneManager::Scene::NIGHT_CYCLE:
    case SceneManager::Scene::END_GAME:
    case SceneManager::Scene::SETTINGS_MENU:
      // TODO: handle settings at some point
      break;
    default:
      break;
  }
  if (selectableEntities.empty()) {
    selectedOptionIterator = std::vector<Entity*>::iterator{};
  } else {
    selectedOptionIterator = selectableEntities.begin();
    updateEntityState(selectedOptionIterator, CardEntity::CardState::SELECTED);
  }
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
      if (keyHoldStart <= 0) {
        keyHoldStart = SDL_GetTicks();
        sceneManager->space_icon->setState(
            SceneManager::SpaceAnimationStates::ACTIVE);
      }
      return SDL_APP_CONTINUE;
    default:
      break;
  }
  return SDL_APP_CONTINUE;
}
void GameManager::longPressAction() {
  switch (getCurrentScene()) {
    case SceneManager::Scene::MAIN_MENU:
      switch (
          dynamic_cast<ButtonEntity*>(*selectedOptionIterator)->getAction()) {
        case ButtonEntity::NEW_RUN:
          studio = StudioState();
          sceneManager->changeScene(SceneManager::Scene::NEW_RUN);
          break;
        case ButtonEntity::SETTINGS:
          sceneManager->changeScene(SceneManager::Scene::SETTINGS_MENU);
          break;
        case ButtonEntity::EXIT_GAME:
          SDL_Quit();
          exit(SDL_APP_SUCCESS);
          break;
        default:
          break;
      }
      break;
    case SceneManager::Scene::NEW_RUN:
      selectCard(SceneManager::NEW_GAME);
      break;
    case SceneManager::Scene::NEW_GAME:
      selectCard(SceneManager::DAY_CYCLE);
      break;
    case SceneManager::Scene::DAY_CYCLE:
      // TODO: store the day case, increment when hitting this state
      selectCard(SceneManager::NIGHT_CYCLE);
      break;
    case SceneManager::Scene::NIGHT_CYCLE:
      // TODO: check day counter against EVENT_TIMED triggers
      // TODO: check day counter against EVENT_RNG triggers

      // DAY or END_CYCLE
      break;
    case SceneManager::Scene::END_CYCLE:
      // CYCLE_UNLOCK or END_GAME
      break;
    case SceneManager::Scene::CYCLE_UNLOCK:
      selectCard(SceneManager::NEW_GAME);
      break;
    case SceneManager::Scene::END_GAME:
      sceneManager->changeScene(SceneManager::MAIN_MENU);
      break;
    case SceneManager::Scene::EVENT_RNG:
      // TODO: set when next EVENT_RNG will occur
      selectCard(SceneManager::NIGHT_CYCLE);
      break;
    case SceneManager::Scene::EVENT_TIMED:
      // TODO: set when next EVENT_TIMED will occur
      selectCard(SceneManager::NIGHT_CYCLE);
      break;
    case SceneManager::Scene::SETTINGS_MENU:
      sceneManager->changeScene(SceneManager::MAIN_MENU);
      break;
    default:
      break;
  }
}
void GameManager::selectCard(SceneManager::Scene nextScene) {
  studio.addCard(dynamic_cast<CardEntity*>(*selectedOptionIterator));
  sceneManager->changeScene(nextScene);
}
void GameManager::shortPressAction() {
  if (selectedOptionIterator == std::vector<Entity*>::iterator{}) {
    return;
  }

  updateEntityState(selectedOptionIterator, CardEntity::CardState::IDLE);
  if (std::next(selectedOptionIterator) == selectableEntities.end()) {
    selectedOptionIterator = selectableEntities.begin();
  } else {
    selectedOptionIterator++;
  }
  updateEntityState(selectedOptionIterator, CardEntity::CardState::SELECTED);
}