#pragma once
#include <SDL3/SDL.h>

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "ButtonEntity.h"
#include "Entity.h"
#include "GameState.h"
#include "TextureManager.h"

class SceneManager {
 public:
  enum Scene {
    MAIN_MENU,
    SETTINGS_MENU,
    NEW_GAME,
    NEW_CYCLE,
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
    CARDS_GAME_MODIFIERS,
    CARDS_EVENTS_RNG,
    CARDS_EVENTS_TIMED,
    CARDS_CYCLE_MODIFIERS,
    CARDS_CYCLE_DAY,
    CARDS_CYCLE_END,
    CYCLE_SUMMARY,
    GAME_SUMMARY,
  };

 private:
  Scene currentScene = MAIN_MENU;
  GameState* state = nullptr;
  TextureManager* textureManager = nullptr;
  std::unordered_map<Scene, std::vector<SceneComponent>> scenes = {};
  std::unordered_map<SceneComponent, std::vector<Entity*>> gameEntities = {};

 public:
  SceneManager(GameState* state, TextureManager* textureManager);
  ~SceneManager();
  void changeScene(Scene newScene);
  Scene getCurrentScene() const;
  std::vector<Entity*> getEntitiesForCurrentScene() const;
  std::vector<Entity*> getEntitiesForSceneComponent(SceneComponent component);

 private:
  ButtonEntity* createMenuButton(
      std::string const& text, int index, int totalButtons,
      ButtonEntity::ButtonState initialState = ButtonEntity::ButtonState::IDLE);
};