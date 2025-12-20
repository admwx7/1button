#pragma once

#include <vector>

#include "GameState.h"

#define STARTING_FUNDS 1000
#define WINNING_FUNDS 1000000
#define WINNING_GAMES 10
#define LOSING_DAYS 1000
#define LOSING_FUNDS 0

class StudioState {
  int day = 1;
  int funds = STARTING_FUNDS;
  std::vector<GameState> games = {};

 public:
  enum Outcome { ONGOING, WIN, LOSE };
  StudioState() { games.push_back(GameState()); }
  ~StudioState() { games.clear(); }
  void addCard(CardEntity* card) { currentGame().addCard(card); }
  GameState& currentGame() { return games.back(); }
  Outcome checkOutcome() const {
    if (funds >= WINNING_FUNDS && games.size() >= WINNING_GAMES) {
      return Outcome::WIN;
    }
    if (funds <= LOSING_FUNDS || day >= LOSING_DAYS) {
      return Outcome::LOSE;
    }
    return Outcome::ONGOING;
  }
};