#include "trainer.h"
#include "algorithm/algorithm.h"
#include "learn/parallelLearningAgent.h"
#include "log/laBasicLogger.h"
#include "log/laPolicyStatsLogger.h"
#include "selector/selector.h"
#include "selectors/truncation.h"
#include <cstddef>
#include <memory>
#include <cstdint>

Trainer::Trainer(GymEnvironment &env, Algorithm::Algorithm &algorithm,
                 std::shared_ptr<Selector::Selector> selector, uint64_t seed, uint64_t nbThreads,
                 bool doValidation, uint64_t maxNbActionsPerEval,
                 size_t maxNbEvaluationPerPolicy,
                 uint64_t nbIterationsPerPolicyEvaluation,
                 uint64_t nbIterationsPerPolicyValidation,
                 uint64_t stepValidation, std::string fileLogs, std::string filePolicyLogs)
    : env(env), currentGeneration(0), algorithm(algorithm), selector(selector), fileLogs(fileLogs), filePolicyLogs(filePolicyLogs) {

  this->algorithm.setSelector(*this->selector);

  this->params.nbThreads = nbThreads;
  this->params.doValidation = doValidation;
  this->params.maxNbActionsPerEval = maxNbActionsPerEval;
  this->params.maxNbEvaluationPerPolicy = maxNbEvaluationPerPolicy;
  this->params.nbIterationsPerPolicyEvaluation =
      nbIterationsPerPolicyEvaluation;
  this->params.nbIterationsPerPolicyValidation =
      nbIterationsPerPolicyValidation;
  this->params.stepValidation = stepValidation;


  if(this->params.maxNbEvaluationPerPolicy < this->params.nbIterationsPerPolicyEvaluation) {
    this->params.maxNbEvaluationPerPolicy = this->params.nbIterationsPerPolicyEvaluation;
  }


  agent = std::make_unique<Learn::ParallelLearningAgent>(env, algorithm, std::make_unique<Learn::LearningParameters>(this->params));
  agent->init(seed);
}

void Trainer::train(int generations) {
  py::gil_scoped_release release;
  Log::LABasicLogger basicLogger = Log::LABasicLogger(*agent);

  std::unique_ptr<Log::LABasicLogger> fileLogger = nullptr;
  std::ofstream logFileStream;
  if(this->fileLogs != "") {
    logFileStream.open(this->fileLogs);
    fileLogger = std::make_unique<Log::LABasicLogger>(*agent, logFileStream);
  }

  std::unique_ptr<Log::LAPolicyStatsLogger> policyLogger = nullptr;
  std::ofstream logPolicyFileStream;
  if(this->filePolicyLogs != "") {
    logPolicyFileStream.open(this->filePolicyLogs);
    policyLogger = std::make_unique<Log::LAPolicyStatsLogger>(*agent, this->algorithm, logPolicyFileStream);
  }

  agent->setNbGen(generations);
  for (int i = 0; i < generations; i++) {
    currentGeneration = i;
    agent->trainOneGeneration(i);
  }
}

void Trainer::step() {
  py::gil_scoped_release release;
  agent->trainOneGeneration(currentGeneration++);
}
