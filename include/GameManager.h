#pragma once
#include <SDL3/SDL.h>

#include <string>
#include <unordered_map>
#include <vector>

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
  enum class GameState {
    MAIN_MENU,
    SETTINGS_MENU,
    NEW_GAME,
    NEW_CYCLE,
    DAY_CYCLE,
    NIGHT_CYCLE,
    END_CYCLE,
    END_GAME,
  };

 public:
  GameManager() {
    // Initialize game manager with renderer
  }

  ~GameManager() {
    // Cleanup resources
  }

  SDL_AppResult handleEvent(SDL_Event* event);
};