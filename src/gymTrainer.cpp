#include "gymTrainer.h"
#include <iostream>

GymTrainer::GymTrainer(GymEnvironment& env,
                       const std::vector<Instructions::Instruction*>& instructions)
    : env(env), currentGeneration(0),
      basicLogger(nullptr), policyStatsLogger(nullptr)
{
    this->params.mutation.tpg.maxInitOutgoingEdges = 2;
    // Build the instruction set from provided list
    for (auto* instr : instructions) {
        if (instr != nullptr) {
            instructionSet.add(*instr);
        } else {
            std::cerr << "[GymTrainer] Warning: null instruction skipped." << std::endl;
        }
    }

    agent = std::make_unique<Learn::ParallelLearningAgent>(env, instructionSet, params);
    agent->init();

    basicLogger = new Log::LABasicLogger(*agent);
}

void GymTrainer::train(int generations) {
    for (int i = 0; i < generations; i++) {
        currentGeneration = i;
        agent->trainOneGeneration(i);
    }
}

void GymTrainer::save(const std::string& path, bool clean) {
    if (clean) {
      agent->keepBestPolicy();
      agent->getTPGGraph()->clearProgramIntrons();
    }

    File::TPGGraphDotExporter dotExporter = File::TPGGraphDotExporter(path.c_str(), *agent->getTPGGraph());
    dotExporter.print();
}

void GymTrainer::load(const std::string& path) {
  File::TPGGraphDotImporter dotImporter = File::TPGGraphDotImporter(path.c_str(), 
                                                                    agent->getTPGGraph()->getEnvironment(), 
                                                                    *agent->getTPGGraph());
	dotImporter.importGraph();
}

void GymTrainer::step() {
    agent->trainOneGeneration(currentGeneration++);
}

void GymTrainer::logStats() {
    TPG::PolicyStats ps;
    ps.setEnvironment(agent->getTPGGraph()->getEnvironment()); // Can't use directly this->env ??
    ps.analyzePolicy(agent->getBestRoot().first);

    std::ofstream bestStats;
    bestStats.open("out_best_stats.md");
    bestStats << ps;
    bestStats.close();
}
