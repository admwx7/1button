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
  selectedOptionIterator = std::vector<Entity*>::iterator{};
  selectableEntities.clear();
  delete sceneManager;
  sceneManager = nullptr;
  delete cardManager;
  cardManager = nullptr;
  if (gameState != nullptr) {
    delete gameState;
    gameState = nullptr;
  }
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
    case SceneManager::Scene::GAME_PLAY: {
      selectableEntities =
          sceneManager->getEntitiesForSceneComponent(SceneManager::GAME_CARDS);
      break;
    }
    case SceneManager::Scene::SETTINGS_MENU:
    case SceneManager::Scene::SUMMARY:
    default:
      // No selectable entities
      // TODO: just move to next scene...
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
    case SceneManager::Scene::MAIN_MENU: {
      switch (
          dynamic_cast<ButtonEntity*>(*selectedOptionIterator)->getAction()) {
        case ButtonEntity::NEW_GAME:
          if (gameState != nullptr) {
            delete gameState;
            gameState = nullptr;
          }
          gameState = new GameState();
          sceneManager->changeScene(SceneManager::Scene::GAME_PLAY);
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
    }
    case SceneManager::Scene::GAME_PLAY: {
      CardEntity* card = dynamic_cast<CardEntity*>(*selectedOptionIterator);
      cardManager->addToStack(card);
      if (cardManager->hasCards()) {
        sceneManager->changeScene(SceneManager::Scene::GAME_PLAY);
      } else {
        cardManager->resolveStack(gameState);
        if (gameState->endRound()) {
          gameState->nextRound();
          cardManager->resetAllDecks();
          sceneManager->changeScene(SceneManager::Scene::GAME_PLAY);
        } else {
          sceneManager->changeScene(SceneManager::Scene::SUMMARY);
        }
      }
      break;
    }
    case SceneManager::Scene::SUMMARY: {
      sceneManager->changeScene(SceneManager::MAIN_MENU);
      break;
    }
    case SceneManager::Scene::SETTINGS_MENU:
    default: {
      sceneManager->changeScene(SceneManager::MAIN_MENU);
      break;
    }
  }
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