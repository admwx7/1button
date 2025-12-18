#include "../include/SceneManager.h"

enum SpaceAnimationStates {
  IDLE,
  ACTIVE,
};

ButtonEntity* SceneManager::createMenuButton(
    const std::string& text, int index, int totalButtons,
    ButtonEntity::ButtonState initialState) {
  float buttonHeight = 32.0f;
  float buttonWidth = 128.0f;
  float buttonGap = 48.0f;

  float totalHeight = totalButtons * (buttonHeight + buttonGap) - buttonGap;
  float startY = (state->windowDimensions.height - totalHeight) / 2.0f;
  float buttonY = startY + index * (buttonHeight + buttonGap);

  SDL_FRect* buttonPosition =
      new SDL_FRect{state->windowDimensions.width / 2.0f - buttonWidth / 2.0f,
                    buttonY, buttonWidth, buttonHeight};

  return new ButtonEntity(
      initialState,
      {
          {ButtonEntity::ButtonState::IDLE,
           new SDL_FRect{0.0f, 0.0f, 128.0f, 32.0f}},
          {ButtonEntity::ButtonState::SELECTED,
           new SDL_FRect{0.0f, 32.0f, 128.0f, 32.0f}},
      },
      textureManager->getTexture(TextureManager::MENU_BUTTON_BACKGROUND),
      buttonPosition, text, {16.0f, 8.0f});
}

SceneManager::SceneManager(GameState* state, TextureManager* textureManager)
    : state(state), textureManager(textureManager) {
  scenes[Scene::MAIN_MENU] = {
      SceneComponent::MAIN_MENU_BACKGROUND,
      SceneComponent::MAIN_MENU_BUTTONS,
      SceneComponent::GLOBAL_HUD,
  };
  scenes[Scene::NEW_GAME] = {
      SceneComponent::GAME_BACKGROUND,
      SceneComponent::CARDS_GAME_MODIFIERS,
      SceneComponent::GLOBAL_HUD,
  };
  scenes[Scene::NEW_CYCLE] = {
      SceneComponent::GAME_BACKGROUND,
      SceneComponent::GAME_HUD,
      SceneComponent::CARDS_CYCLE_MODIFIERS,
      SceneComponent::GLOBAL_HUD,
  };
  scenes[Scene::DAY_CYCLE] = {
      SceneComponent::GAME_BACKGROUND,
      SceneComponent::GAME_HUD,
      SceneComponent::CARDS_CYCLE_DAY,
      SceneComponent::GLOBAL_HUD,
  };
  scenes[Scene::NIGHT_CYCLE] = {
      SceneComponent::GAME_BACKGROUND,
      SceneComponent::GAME_HUD,
      SceneComponent::GLOBAL_HUD,
  };
  scenes[Scene::END_CYCLE] = {
      SceneComponent::GAME_BACKGROUND,
      SceneComponent::GAME_HUD,
      SceneComponent::CYCLE_SUMMARY,
      SceneComponent::GLOBAL_HUD,
  };
  scenes[Scene::CYCLE_UNLOCK] = {
      SceneComponent::GAME_BACKGROUND,
      SceneComponent::GAME_HUD,
      SceneComponent::CARDS_CYCLE_END,
      SceneComponent::GLOBAL_HUD,
  };
  scenes[Scene::END_GAME] = {
      SceneComponent::GAME_BACKGROUND,
      SceneComponent::GAME_SUMMARY,
      SceneComponent::GLOBAL_HUD,
  };
  scenes[Scene::EVENT_TIMED] = {
      SceneComponent::GAME_BACKGROUND,
      SceneComponent::GAME_HUD,
      SceneComponent::CARDS_EVENTS_TIMED,
      SceneComponent::GLOBAL_HUD,
  };
  scenes[Scene::EVENT_RNG] = {
      SceneComponent::GAME_BACKGROUND,
      SceneComponent::GAME_HUD,
      SceneComponent::CARDS_EVENTS_RNG,
      SceneComponent::GLOBAL_HUD,
  };

  gameEntities[SceneComponent::MAIN_MENU_BACKGROUND] = {new Entity(
      textureManager->getTexture(TextureManager::MENU_BACKGROUND), NULL, NULL)};
  gameEntities[SceneComponent::GAME_BACKGROUND] = {new Entity(
      textureManager->getTexture(TextureManager::GAME_BACKGROUND), NULL, NULL)};
  gameEntities[SceneComponent::MAIN_MENU_BUTTONS] = {
      createMenuButton("New Game", 0, 3, ButtonEntity::ButtonState::SELECTED),
      createMenuButton("Settings", 1, 3),
      createMenuButton("Quit", 2, 3),
  };
  gameEntities[SceneComponent::GLOBAL_HUD] = {new Sprite<SpaceAnimationStates>(
      textureManager->getTexture(TextureManager::SPACE_BAR_ICON),
      new SDL_FRect{state->windowDimensions.width - 40.0f,
                    state->windowDimensions.height - 40.0f, 32.0f, 32.0f},
      {
          {SpaceAnimationStates::IDLE,
           {new SDL_FRect{0.0f, 0.0f, 32.0f, 32.0f}}},
          {SpaceAnimationStates::ACTIVE,
           {new SDL_FRect{256.0f, 0.0f, 32.0f, 32.0f}}},
      },
      SpaceAnimationStates::IDLE)};
  gameEntities[SceneComponent::GAME_HUD] = {};
  gameEntities[SceneComponent::CARDS_GAME_MODIFIERS] = {};
  gameEntities[SceneComponent::CARDS_EVENTS_RNG] = {};
  gameEntities[SceneComponent::CARDS_EVENTS_TIMED] = {};
  gameEntities[SceneComponent::CARDS_CYCLE_MODIFIERS] = {};
  gameEntities[SceneComponent::CARDS_CYCLE_DAY] = {};
  gameEntities[SceneComponent::CARDS_CYCLE_END] = {};
  gameEntities[SceneComponent::CYCLE_SUMMARY] = {};
  gameEntities[SceneComponent::GAME_SUMMARY] = {};
}
SceneManager::~SceneManager() {
  for (auto& [state, entities] : gameEntities) {
    for (Entity* entity : entities) {
      delete entity;
    }
  }
}
void SceneManager::changeScene(Scene newScene) { currentScene = newScene; }
SceneManager::Scene SceneManager::getCurrentScene() const {
  return currentScene;
}
std::vector<Entity*> SceneManager::getEntitiesForCurrentScene() const {
  std::vector<Entity*> entities = {};
  for (auto& component : scenes.at(currentScene)) {
    entities.insert(entities.end(), gameEntities.at(component).begin(),
                    gameEntities.at(component).end());
  }
  return entities;
}
std::vector<Entity*> SceneManager::getEntitiesForSceneComponent(
    SceneComponent component) {
  return gameEntities.at(component);
}