#pragma once
#include <memory>
#include <vector>

#include "CardEntity.h"

class GameState {
  int target = 10;
  int round = 1;
  int score = 0;
  int roundScore = 0;

 public:
  GameState() {}
  void nextRound() {
    round++;
    target = target * round;
  }
  void addScore(int points) { roundScore += points; }
  void setScore(int points) { roundScore = points; }
  void setTarget(int newTarget) { target = newTarget; }
  int getTarget() const { return target; }
  int getRound() const { return round; }
  int getScore() const { return roundScore; }
  bool endRound() {
    score += roundScore;
    roundScore = 0;
    print();
    return score >= target;
  }
  void print() {
    SDL_Log("Round: %d, Target: %d, Score: %d, RoundScore: %d", round, target,
            score, roundScore);
  }
};