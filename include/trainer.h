#pragma once
#include "algorithm/algorithm.h"
#include "environments/gymEnvironment.h"
#include "learn/learningAgent.h"
#include "parameters.h"

class Trainer {
public:
  Trainer(GymEnvironment &env,
             Algorithm::Algorithm &algorithm);

  void train(int generations);
  void step();

private:
  const GymEnvironment &env;
  Parameters params;

  Algorithm::Algorithm &algorithm;
  std::unique_ptr<Learn::LearningAgent> agent;
  int currentGeneration;
};
