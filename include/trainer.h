#pragma once
#include "algorithm/algorithm.h"
#include "environments/gymEnvironment.h"
#include "learn/learningAgent.h"
#include "parameters.h"

class Trainer {
public:
  Trainer(GymEnvironment &env, Algorithm::Algorithm &algorithm,
          std::shared_ptr<Selector::Selector> selector,  uint64_t seed, uint64_t nbThreads, bool doValidation,
          uint64_t maxNbActionsPerEval, size_t maxNbEvaluationPerPolicy,
          uint64_t nbIterationsPerPolicyEvaluation,
          uint64_t nbIterationsPerPolicyValidation, uint64_t stepValidation, std::string fileLogs, std::string filePolicyLogs);

  void train(int generations);
  void step();

private:
  const GymEnvironment &env;
  Learn::LearningParameters params;

  Algorithm::Algorithm &algorithm;
  std::unique_ptr<Learn::LearningAgent> agent;
  std::shared_ptr<Selector::Selector> selector;
  int currentGeneration;

  std::string filePolicyLogs;
  std::string fileLogs;
};
