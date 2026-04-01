#include "trainer.h"
#include "learn/parallelLearningAgent.h"
#include "log/laBasicLogger.h"

Trainer::Trainer(GymEnvironment &env, Algorithm::Algorithm &algorithm)
    : env(env), currentGeneration(0), algorithm(algorithm) {
  agent =
      std::make_unique<Learn::ParallelLearningAgent>(env, algorithm, params.evaluation);
  agent->init();
}

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

