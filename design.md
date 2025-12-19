# TODO
- [x] web build asset
- [x] create our itch.io page
- [x] upload build
- [ ] finish prototype
  - [x] animation triggers for Sprite
  - [x] animate the space bar icon when holding space
  - [ ] add in card sets
  - [ ] add core game logic
    - [~] state machine for phases
    - [~] track user actions / stats
    - [ ] hooks for card logic
    - [ ] win/loss state conditionals
- [ ] add polish
  - [ ] tutorials / hints for how to play
  - [ ] add "upgrade" system
  - [ ] replace placeholder assets with literally anything
  - [ ] SFX for interactions
  - [ ] Background music
  - [ ] expand card options - aka more content
  - [ ] balancing
  - [ ] add in scene transitions
  - [ ] replace background sprites with something interesting
  - [ ] add some animation to sprites
- [ ] upload build
- [ ] refine our itch.io page

# Requirements

## Technical
### Must Have
- virtual sizing
- web build (WebGL)
### Nice to Have
- multi-threading for game state | renderer
- basic animation (given time)

## Theme: GameDev
- 16 bit graphics
- cards
  - overlay, simple card style
- environment:
  - menu / inventory style
- color palette: bright / vibrant / fun / warm



# CONCEPT
- https://gameaccessibilityguidelines.com/

Concept: <INSERT PRODUCT HERE> Tycoon
Genre: Simulation | Roguelike
Controls: 1-Button, cycle cards, long-press to select
Inspiration: Roguelike Deckbuilder + Business Tycoon

GAME MODIFIERS | persist for the full run
- high industry turnover | people are more likely to quit
- difficulty level | can't fall below X employees

CYCLE MODIFIERS | change periodically throughout a run
-- time cycle --
  -- RNG (7-14d) --
  [1] - lead dev quit \/ productivity | new hire
  -- STATIC (30d) --
  - pay bills (including salaries)
-- product cycle --
  SETUP
  CORE GAMEPLAY LOOP
  CONCLUSION => REPEAT

SETUP - once per product cycle
-- choose a product to build --
[1] [2] [3] - build your own <insert> | might include 5+ card selections

CORE GAMEPLAY LOOP - repeats until loss condition or product completion
-- game creation --
[1] [2] [3] - choose a genre | ^ Complexity
[1] [2] [3] - choose a mechanical system | MMO, Single player, ...
-- day/night (repeat as needed) --
[1] [2] [3] - operations | ^ productivity   \/ morale

CONCLUSION - once per product cycle, can trigger win conditions
- upgrade system: [1] [2] [3] - unlock rpgs | upgrade computers | ...
- payout: $$$
- level up: employees

POST GAME
- summary

WIN CONDITIONS
- deliver X products
- earn X currency

LOSE CONDITIONS
- all employees quit
- ran out of funding
