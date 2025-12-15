# Requirements

## Technical
### Must Have
- multi-threading for game state | renderer
- virtual sizing
- web build (WebGL)
### Nice to Have
- basic animation (given time)

## Theme: GameDev
- 16 bit graphics
- cards
  - overlay, simple card style
- environment:
  - menu / inventory style
- color palette: bright / vibrant / fun / warm

# Code Structure

## SINGLETON: ScreenManager - defines the API for screens to render out
  - maps GameState from GameManager to the appropriate Screen for rendering
  - connects to SDL renderer and GameManager
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
## ENUM: GameState
  - MainMenu
  - SettingsMenu
  - NewGame | Set game modifiers
  - NewCycle | Set cycle modifiers
  - DayCycle | Make operational decisions
  - NightCycle | See the outcomes
  - EndCycle | Cycle summary, payouts, leveling, ...
  - EndGame | Game summary, score, stats
## ENUM: CardState
  - IDLE
  - SELECT
## CLASS: Sprite<T> extend Object
  - AnimationMap: Map<T, vector<SDL_RectF>>
  - FrameRate
  - CurrentFrame
## CLASS: Object
  - Texture: SDL_Texture | sprite map
  - Position: SDL_RectF | where on screen
## CLASS: TextObject extend Object
  - Text: string
  - Font: SDL_Font
  - TextPosition: SDL_RectF