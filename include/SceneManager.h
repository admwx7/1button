#pragma once
#include <SDL3/SDL.h>

#include <functional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "ButtonEntity.h"
#include "CardEntity.h"
#include "CardManager.h"
#include "Entity.h"
#include "GlobalState.h"
#include "Sprite.h"
#include "TextureManager.h"

class SceneManager {
 public:
  enum Scene {
    MAIN_MENU,
    SETTINGS_MENU,
    GAME_PLAY,
    SUMMARY,
  };
  enum SceneComponent {
    MAIN_MENU_BACKGROUND,
    MAIN_MENU_BUTTONS,
    GLOBAL_HUD,
    GAME_BACKGROUND,
    GAME_HUD,
    GAME_CARDS,
    GAME_SUMMARY,
  };
  enum SpaceAnimationStates {
    IDLE,
    ACTIVE,
  };

 private:
  Scene currentScene = MAIN_MENU;
  GlobalState* state = nullptr;
  TextureManager* textureManager = nullptr;
  CardManager* cardManager = nullptr;
  std::unordered_map<Scene, std::vector<SceneComponent>> scenes = {};
  std::unordered_map<SceneComponent, std::vector<Entity*>> gameEntities = {};
  std::function<void(void)> sceneChangeCallback = nullptr;

 public:
  Sprite<SpaceAnimationStates>* space_icon = nullptr;

  SceneManager(GlobalState* state, TextureManager* textureManager,
               CardManager* cardManager, std::function<void(void)> callback);
  ~SceneManager();
  void changeScene(Scene newScene);
  Scene getCurrentScene() const;
  std::vector<Entity*> getEntitiesForCurrentScene() const;
  std::vector<Entity*> getEntitiesForSceneComponent(SceneComponent component);

 private:
  ButtonEntity* createMenuButton(
      ButtonEntity::ButtonAction action, int index, int totalButtons,
      ButtonEntity::ButtonState initialState = ButtonEntity::ButtonState::IDLE);
};