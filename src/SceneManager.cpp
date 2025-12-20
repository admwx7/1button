#include "../include/SceneManager.h"

ButtonEntity* SceneManager::createMenuButton(
    ButtonEntity::ButtonAction action, int index, int totalButtons,
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
      buttonPosition, action, {16.0f, 8.0f});
}

SceneManager::SceneManager(GlobalState* state, TextureManager* textureManager,
                           CardManager* cardManager,
                           std::function<void(void)> callback)
    : state(state),
      textureManager(textureManager),
      cardManager(cardManager),
      sceneChangeCallback(callback) {
  space_icon = new Sprite<SpaceAnimationStates>(
      textureManager->getTexture(TextureManager::SPACE_BAR_ICON),
      new SDL_FRect{state->windowDimensions.width - 40.0f,
                    state->windowDimensions.height - 40.0f, 32.0f, 32.0f},
      {
          {SpaceAnimationStates::IDLE,
           {new SDL_FRect{0.0f, 0.0f, 32.0f, 32.0f}}},
          {SpaceAnimationStates::ACTIVE,
           {
               new SDL_FRect{0.0f, 0.0f, 32.0f, 32.0f},
               new SDL_FRect{0.0f, 0.0f, 32.0f, 32.0f},
               new SDL_FRect{0.0f, 0.0f, 32.0f, 32.0f},
               new SDL_FRect{32.0f, 0.0f, 32.0f, 32.0f},
               new SDL_FRect{64.0f, 0.0f, 32.0f, 32.0f},
               new SDL_FRect{96.0f, 0.0f, 32.0f, 32.0f},
               new SDL_FRect{128.0f, 0.0f, 32.0f, 32.0f},
               new SDL_FRect{160.0f, 0.0f, 32.0f, 32.0f},
               new SDL_FRect{192.0f, 0.0f, 32.0f, 32.0f},
               new SDL_FRect{224.0f, 0.0f, 32.0f, 32.0f},
               new SDL_FRect{256.0f, 0.0f, 32.0f, 32.0f},
           }},
      },
      SpaceAnimationStates::IDLE, 100, false);

  scenes[Scene::MAIN_MENU] = {
      SceneComponent::MAIN_MENU_BACKGROUND,
      SceneComponent::MAIN_MENU_BUTTONS,
      SceneComponent::GLOBAL_HUD,
  };
  scenes[Scene::GAME_PLAY] = {
      SceneComponent::GAME_BACKGROUND,
      SceneComponent::GAME_HUD,
      SceneComponent::GAME_CARDS,
      SceneComponent::GLOBAL_HUD,
  };
  scenes[Scene::SUMMARY] = {
      SceneComponent::GAME_BACKGROUND,
      SceneComponent::GAME_HUD,
      SceneComponent::GAME_SUMMARY,
      SceneComponent::GLOBAL_HUD,
  };

  gameEntities[SceneComponent::MAIN_MENU_BACKGROUND] = {new Entity(
      textureManager->getTexture(TextureManager::MENU_BACKGROUND), NULL, NULL)};
  gameEntities[SceneComponent::GAME_BACKGROUND] = {new Entity(
      textureManager->getTexture(TextureManager::GAME_BACKGROUND), NULL, NULL)};
  gameEntities[SceneComponent::MAIN_MENU_BUTTONS] = {
      createMenuButton(ButtonEntity::NEW_GAME, 0, 3,
                       ButtonEntity::ButtonState::SELECTED),
      // createMenuButton(ButtonEntity::SETTINGS, 1, 3),
      createMenuButton(ButtonEntity::EXIT_GAME, 2, 3),
  };
  gameEntities[SceneComponent::GLOBAL_HUD] = {space_icon};
  gameEntities[SceneComponent::GAME_HUD] = {};
  gameEntities[SceneComponent::GAME_SUMMARY] = {};
}
SceneManager::~SceneManager() {
  for (auto& [state, entities] : gameEntities) {
    for (auto entity : entities) {
      delete entity;
    }
  }
  gameEntities.clear();
}
void SceneManager::changeScene(Scene newScene) {
  SceneComponent sceneComponent;
  CardManager::DeckType deckType;

  switch (newScene) {
    case GAME_PLAY:
      sceneComponent = SceneComponent::GAME_CARDS;
      deckType = CardManager::DeckType::AVAILABLE;
      gameEntities[sceneComponent].clear();

      // Draw new cards
      for (auto& card : cardManager->drawCards(deckType)) {
        gameEntities[sceneComponent].push_back(card);
      }
      break;
    case SUMMARY:
    case MAIN_MENU:
    default:
      break;
  }
  currentScene = newScene;
  if (sceneChangeCallback != nullptr) {
    sceneChangeCallback();
  }
}
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