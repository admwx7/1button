#pragma once
#include <SDL3/SDL.h>
#include <assert.h>

#include "CardManager.h"
#include "Entity.h"
#include "GlobalState.h"
#include "SceneManager.h"
#include "TextEntity.h"
#include "TextureManager.h"

class GameManager {
 private:
  GlobalState* state = nullptr;
  SceneManager* sceneManager = nullptr;
  TextureManager* textureManager = nullptr;
  CardManager* cardManager = nullptr;
  GameState* gameState = nullptr;
  uint64_t keyHoldStart = 0;                 // milliseconds
  const uint64_t KEY_HOLD_THRESHOLD = 1000;  // milliseconds
  std::vector<Entity*> selectableEntities = {};
  std::vector<Entity*>::iterator selectedOptionIterator = {};

  void sceneChangedCallback();

 protected:
  void longPressAction();
  void shortPressAction();

 public:
  GameManager(GlobalState* state, TextureManager* textureManager);
  ~GameManager();

  SceneManager::Scene getCurrentScene() const;
  std::vector<Entity*> getEntitiesForCurrentScene() const;
  SDL_AppResult handleEvent(SDL_Event* event);
};