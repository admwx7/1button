# TODO
- [x] web build asset
- [x] create our itch.io page
- [x] upload build
- [x] finish prototype
  - [x] animation triggers for Sprite
  - [x] animate the space bar icon when holding space
  - [x] add core game logic
    - [x] state machine for phases
    - [x] track user actions / stats
    - [x] hooks for card logic
    - [x] win/loss state conditionals
- [ ] Polish
  - [ ] Add Round, Target, Current and Total Scores to global HUD
  - [ ] Show the selected cards along right side of HUD
  - [ ] Add details to Summary page
  - [ ] Replace background with something nicer
  - [ ] Show a shuffling animation between rounds
  - [ ] Add an animation of cards being added together
- [ ] upload build
- [ ] refine our itch.io page

# FIX
- [ ] memory leak when closing game manually during a round

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
## Set target
- pts to earn: 10
- each round is a multiplier, prev target * (round + 1)
## Loop
- draw 3
- select 1, add to stack
- discard remaining
- repeat until no cards remain
## Resolve stack
- order matters

# Card Options
- value cards: +1, +2, +3
  - add to running total
- multiplier cards: x2, x3
  - multiplier on running total
- special cards:
  - echo (clone last card)
  - worm (reduce pts to earn: req pts * 0.1, round down)