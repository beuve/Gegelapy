#include "trainer.h"
#include "algorithm/algorithm.h"
#include "learn/parallelLearningAgent.h"
#include "log/laBasicLogger.h"
#include "selector/selector.h"
#include "selectors/truncation.h"
#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

Trainer::Trainer(
    GymEnvironment &env,
    std::vector<std::reference_wrapper<Algorithm::Algorithm>> algorithms,
    std::shared_ptr<Selector::Selector> selector, uint64_t seed,
    bool doValidation, uint64_t maxNbActionsPerEval,
    size_t maxNbEvaluationPerPolicy, uint64_t nbIterationsPerPolicyEvaluation,
    uint64_t nbIterationsPerPolicyValidation, uint64_t stepValidation)
    : env(env), currentGeneration(0), algorithms(algorithms),
      selector(selector) {

  for (auto &alg : this->algorithms) {
    alg.get().setSelector(*this->selector);
  }

  this->params.evaluation.doValidation = doValidation;
  this->params.evaluation.maxNbActionsPerEval = maxNbActionsPerEval;
  this->params.evaluation.maxNbEvaluationPerPolicy = maxNbEvaluationPerPolicy;
  this->params.evaluation.nbIterationsPerPolicyEvaluation =
      nbIterationsPerPolicyEvaluation;
  this->params.evaluation.nbIterationsPerPolicyValidation =
      nbIterationsPerPolicyValidation;
  this->params.evaluation.stepValidation = stepValidation;

  agent = std::make_unique<Learn::ParallelLearningAgent>(env, this->algorithms);
  agent->init(seed);
}

Trainer::Trainer(GymEnvironment &env, Algorithm::Algorithm &algorithm,
                 std::shared_ptr<Selector::Selector> selector, uint64_t seed,
                 bool doValidation, uint64_t maxNbActionsPerEval,
                 size_t maxNbEvaluationPerPolicy,
                 uint64_t nbIterationsPerPolicyEvaluation,
                 uint64_t nbIterationsPerPolicyValidation,
                 uint64_t stepValidation)
    : Trainer(env, std::vector<std::reference_wrapper<Algorithm::Algorithm>>{algorithm}, selector, seed, doValidation,
              maxNbActionsPerEval, maxNbEvaluationPerPolicy,
              nbIterationsPerPolicyEvaluation, nbIterationsPerPolicyValidation,
              stepValidation) {}

void Trainer::train(int generations) {
  py::gil_scoped_release release;
  Log::LABasicLogger basicLogger = Log::LABasicLogger(*agent);
  for (int i = 0; i < generations; i++) {
    currentGeneration = i;
    agent->trainOneGeneration(i);
  }
}

void Trainer::step() {
  py::gil_scoped_release release;
  agent->trainOneGeneration(currentGeneration++);
}
