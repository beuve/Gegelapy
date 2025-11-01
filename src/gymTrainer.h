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
               const std::vector<Instructions::Instruction*>& instructions,
               size_t archive_size = 50,
               double archiving_probability = 0.05,
               uint64_t nb_iterations_per_policy_evaluation = 5,
               uint64_t nb_iterations_per_policy_validation = 5,
               uint64_t max_nb_actions_per_eval = 1000,
               double ratio_deleted_roots = 0.5,
               uint64_t size_tournament = 5,
               uint64_t nb_generations = 500,
               size_t max_nb_evaluation_per_policy = 1000,
               size_t nb_iterations_per_job = 1,
               size_t nb_registers = 8,
               size_t nb_program_constant = 0,
               std::string activation_function = "none",
               bool do_validation = false,
               uint64_t step_validation = 1,
               size_t nb_roots = 100,
               double ratio_teams_over_actions = 1.0,
               size_t max_init_outgoing_edges = 3,
               double prob_change_action_class = 0.1,
               double prob_action_edge_deletion = 0.7,
               double prob_action_edge_addition = 0.7,
               double prob_mutate_action_program = 0.1,
               double prob_swap_action_program = 0.1,
               size_t nb_action_edge_init = 1,
               size_t max_outgoing_edges = 5,
               double prob_edge_deletion = 0.7,
               double prob_edge_addition = 0.7,
               double prob_program_mutation = 0.2,
               double prob_context_over_action_program = 0.5,
               bool force_program_behavior_change_on_mutation = false,
               double prob_edge_destination_change = 0.1,
               double prob_edge_destination_is_action = 0.5,
               bool use_action_program = false,
               bool use_multi_action_program = false,
               bool team_access_all_actions = true,
               size_t max_program_size = 96,
               size_t init_min_program_size = 1,
               size_t init_max_program_size = 10,
               double prob_delete = 0.5,
               double prob_add = 0.5,
               double prob_mutate = 1.0,
               double prob_swap = 1.0,
               double prob_constant_mutation = 0.5,
               double prob_new_program = 0.0,
               double min_const_value = -1.0,
               double max_const_value = 1.0);

    void train(int generations);
    void save(const char* path, bool clean = false);
    void load(const char* path);
    void step();

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
