#pragma once
#include "learn/learningAgent.h"
#include "learn/parallelLearningAgent.h"
#include "instructions/set.h"
#include "log/laBasicLogger.h"
#include "log/laPolicyStatsLogger.h"
#include "file/parametersParser.h"
#include "file/tpgGraphDotExporter.h"
#include "file/tpgGraphDotImporter.h"
#include "gymEnvironment.h"

#include <memory>
#include <atomic>
#include <thread>
#include <fstream>

class GymTrainer {
public:
    GymTrainer(GymEnvironment& env,
               const std::vector<Instructions::Instruction*>& instructions);

    void train(int generations);
    void save(const std::string& path, bool clean = false);
    void load(const std::string& path);
    void step();
    void logStats();

private:
    const GymEnvironment& env;
    Learn::LearningParameters params;
    Instructions::Set instructionSet;
    std::unique_ptr<Learn::LearningAgent> agent;

    Log::LABasicLogger* basicLogger;
    Log::LAPolicyStatsLogger* policyStatsLogger;
    std::ofstream statsFile;

    int currentGeneration;
};
