#pragma once
#include <SDL3/SDL.h>

#include "Entity.h"
#include "GameState.h"
#include "SceneManager.h"
#include "TextEntity.h"
#include "TextureManager.h"

/*
## SINGLETON: GameManager - manages the overall game state
  - track all run details
    - modifier, decks, cycle, score, ...
  - methods to fetch options from deck
  - method to select an option
  - keep history details (actions taken, outcomes, ...)
  - current GameState
    - provides GameState to ScreenManager to trigger renderer
    - houses all state machine logic to transition between GameStates
  - connects to SDL event handler
    - show options for cards
    - transition between currently selected card
*/

class GameManager {
 private:
  GameState* state = nullptr;
  SceneManager* sceneManager = nullptr;
  TextureManager* textureManager = nullptr;
  uint64_t keyHoldStart = 0;                 // milliseconds
  const uint64_t KEY_HOLD_THRESHOLD = 1000;  // milliseconds
  // TODO: ensure this is reset to 0 when scene is changed
  int selectedOptionIndex = 0;

 protected:
  void longPressAction();
  void shortPressAction();

 public:
  GameManager(GameState* state, TextureManager* textureManager);
  ~GameManager();

  SceneManager::Scene getCurrentScene() const;
  std::vector<Entity*> getEntitiesForCurrentScene() const;
  SDL_AppResult handleEvent(SDL_Event* event);
};