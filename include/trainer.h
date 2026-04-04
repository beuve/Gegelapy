#pragma once
#include "algorithm/algorithm.h"
#include "environments/gymEnvironment.h"
#include "learn/learningAgent.h"
#include "parameters.h"
#include <vector>

class Trainer {
public:
  Trainer(GymEnvironment &env, Algorithm::Algorithm &algorithm,
          std::shared_ptr<Selector::Selector> selector, uint64_t seed,
          bool doValidation, uint64_t maxNbActionsPerEval,
          size_t maxNbEvaluationPerPolicy,
          uint64_t nbIterationsPerPolicyEvaluation,
          uint64_t nbIterationsPerPolicyValidation, uint64_t stepValidation);

  Trainer(GymEnvironment &env, std::vector<std::reference_wrapper<Algorithm::Algorithm>> algorithms,
          std::shared_ptr<Selector::Selector> selector, uint64_t seed,
          bool doValidation, uint64_t maxNbActionsPerEval,
          size_t maxNbEvaluationPerPolicy,
          uint64_t nbIterationsPerPolicyEvaluation,
          uint64_t nbIterationsPerPolicyValidation, uint64_t stepValidation);

  void train(int generations);
  void step();

private:
  const GymEnvironment &env;
  Parameters params;

  std::vector<std::reference_wrapper<Algorithm::Algorithm>> algorithms;
  std::unique_ptr<Learn::LearningAgent> agent;
  std::shared_ptr<Selector::Selector> selector;
  int currentGeneration;
};
