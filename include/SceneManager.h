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
#include "GameState.h"
#include "Sprite.h"
#include "TextureManager.h"

class SceneManager {
 public:
  enum Scene {
    MAIN_MENU,
    SETTINGS_MENU,
    NEW_RUN,
    NEW_GAME,
    NEW_GAME_GENRE,
    NEW_GAME_MECHANIC,
    DAY_CYCLE,
    NIGHT_CYCLE,
    END_CYCLE,
    CYCLE_UNLOCK,
    END_GAME,
    EVENT_TIMED,
    EVENT_RNG,
  };
  enum SceneComponent {
    MAIN_MENU_BACKGROUND,
    GLOBAL_HUD,
    MAIN_MENU_BUTTONS,
    GAME_BACKGROUND,
    GAME_HUD,
    CARDS_RUN_MODIFIERS,
    CARDS_EVENTS_RNG,
    CARDS_EVENTS_TIMED,
    CARDS_GAME_MODIFIERS,
    CARDS_GAME_GENRES,
    CARDS_GAME_MECHANICS,
    CARDS_DAY,
    CARDS_UNLOCK,
    GAME_SUMMARY,
    RUN_SUMMARY,
  };
  enum SpaceAnimationStates {
    IDLE,
    ACTIVE,
  };

 private:
  Scene currentScene = MAIN_MENU;
  GameState* state = nullptr;
  TextureManager* textureManager = nullptr;
  CardManager* cardManager = nullptr;
  std::unordered_map<Scene, std::vector<SceneComponent>> scenes = {};
  std::unordered_map<SceneComponent, std::vector<Entity*>> gameEntities = {};
  std::function<void(void)> sceneChangeCallback = nullptr;

 public:
  Sprite<SpaceAnimationStates>* space_icon = nullptr;

  SceneManager(GameState* state, TextureManager* textureManager,
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