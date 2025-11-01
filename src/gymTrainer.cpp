#include "gymTrainer.h"
#include "codeGen/tpgGenerationEngineFactory.h"
#include "codeGen/tpgGenerationEngine.h"
#include <iostream>

GymTrainer::GymTrainer(
  GymEnvironment& env, const std::vector<Instructions::Instruction*>& instructions,
  size_t archive_size, double archiving_probability, 
  uint64_t nb_iterations_per_policy_evaluation, uint64_t nb_iterations_per_policy_validation, 
  uint64_t max_nb_actions_per_eval, double ratio_deleted_roots, uint64_t size_tournament, 
  uint64_t nb_generations, size_t max_nb_evaluation_per_policy, size_t nb_iterations_per_job, 
  size_t nb_registers, size_t nb_program_constant, std::string activation_function, 
  bool do_validation, uint64_t step_validation, size_t nb_roots, 
  double ratio_teams_over_actions, size_t max_init_outgoing_edges, 
  double prob_change_action_class, double prob_action_edge_deletion, 
  double prob_action_edge_addition, double prob_mutate_action_program, 
  double prob_swap_action_program, size_t nb_action_edge_init, size_t max_outgoing_edges, 
  double prob_edge_deletion, double prob_edge_addition, double prob_program_mutation, 
  double prob_context_over_action_program, bool force_program_behavior_change_on_mutation, 
  double prob_edge_destination_change, double prob_edge_destination_is_action, 
  bool use_action_program, bool use_multi_action_program, bool team_access_all_actions, 
  size_t max_program_size, size_t init_min_program_size, size_t init_max_program_size, 
  double prob_delete, double prob_add, double prob_mutate, double prob_swap, 
  double prob_constant_mutation, double prob_new_program, double min_const_value, 
  double max_const_value)
    : env(env), currentGeneration(0),
      basicLogger(nullptr), policyStatsLogger(nullptr)
{
    this->params.archiveSize = archive_size;
    this->params.archivingProbability = archiving_probability;
    this->params.nbIterationsPerPolicyEvaluation = nb_iterations_per_policy_evaluation;
    this->params.nbIterationsPerPolicyValidation = nb_iterations_per_policy_validation;
    this->params.maxNbActionsPerEval = max_nb_actions_per_eval;
    this->params.ratioDeletedRoots = ratio_deleted_roots;
    this->params.sizeTournament = size_tournament;
    this->params.nbGenerations = nb_generations;
    this->params.maxNbEvaluationPerPolicy = max_nb_evaluation_per_policy;
    this->params.nbIterationsPerJob = nb_iterations_per_job;
    this->params.nbProgramConstant = nb_program_constant;
    this->params.activationFunction = activation_function;
    this->params.doValidation = do_validation;
    this->params.stepValidation = step_validation;
    // TPG
    this->params.mutation.tpg.nbRoots = nb_roots;
    this->params.mutation.tpg.ratioTeamsOverActions = ratio_teams_over_actions;
    this->params.mutation.tpg.maxInitOutgoingEdges = max_init_outgoing_edges;
    this->params.mutation.tpg.pChangeActionClass = prob_change_action_class;
    this->params.mutation.tpg.pActionEdgeDeletion = prob_action_edge_deletion;
    this->params.mutation.tpg.pMutateActionProgram = prob_mutate_action_program;
    this->params.mutation.tpg.pSwapActionProgram = prob_swap_action_program;
    this->params.mutation.tpg.nbActionEdgeInit = nb_action_edge_init;
    this->params.mutation.tpg.maxOutgoingEdges = max_outgoing_edges;
    this->params.mutation.tpg.pEdgeDeletion = prob_edge_deletion;
    this->params.mutation.tpg.pProgramMutation = prob_program_mutation;
    this->params.mutation.tpg.probaContextOverActionProgram = prob_context_over_action_program;
    this->params.mutation.tpg.forceProgramBehaviorChangeOnMutation = force_program_behavior_change_on_mutation;
    this->params.mutation.tpg.pEdgeDestinationChange = prob_edge_destination_change;
    this->params.mutation.tpg.pEdgeDestinationIsAction = prob_edge_destination_is_action;
    this->params.mutation.tpg.useActionProgram = use_action_program;
    this->params.mutation.tpg.useMultiActionProgram = use_multi_action_program;
    this->params.mutation.tpg.teamAccessAllActions = team_access_all_actions;
    // Program
    this->params.mutation.prog.maxProgramSize = max_program_size;
    this->params.mutation.prog.initMinProgramSize = init_min_program_size;
    this->params.mutation.prog.initMaxProgramSize = init_max_program_size;
    this->params.mutation.prog.pDelete = prob_delete;
    this->params.mutation.prog.pAdd = prob_add;
    this->params.mutation.prog.pMutate = prob_mutate;
    this->params.mutation.prog.pSwap = prob_swap;
    this->params.mutation.prog.pConstantMutation = prob_constant_mutation;
    this->params.mutation.prog.pNewProgram = prob_new_program;
    this->params.mutation.prog.minConstValue = min_const_value;
    this->params.mutation.prog.maxConstValue = max_const_value;

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

void GymTrainer::save(const char* path, bool clean) {
    if (clean) {
      agent->keepBestPolicy();
      agent->getTPGGraph()->clearProgramIntrons();
    }

    File::TPGGraphDotExporter dotExporter = File::TPGGraphDotExporter(path, *agent->getTPGGraph());
    dotExporter.print();
}

void GymTrainer::load(const char* path) {
  File::TPGGraphDotImporter dotImporter = File::TPGGraphDotImporter(path, 
                                                                    agent->getTPGGraph()->getEnvironment(), 
                                                                    *agent->getTPGGraph());
	dotImporter.importGraph();
}

void GymTrainer::step() {
    agent->trainOneGeneration(currentGeneration++);
}

void GymTrainer::compile(const char* path) {
  namespace fs = std::filesystem;

  fs::path dest_dir(path);
  fs::path dest_src_dir = dest_dir / "src";

  // Is this necessary?
  agent->keepBestPolicy();
  agent->getTPGGraph()->clearProgramIntrons();

  // Get gegelapy installation path to retrieve the codegen folder
  py::gil_scoped_acquire acquire;
  py::object gegelapy_module = py::module::import("gegelapy");
  fs::path gegelapy_path = gegelapy_module.attr("__file__").cast<std::string>();
  std::filesystem::path codegen_src = fs::path(gegelapy_path).parent_path() / "codegen";

  // Copy the codegen folder to the destination path
  const auto copyOptions = fs::copy_options::recursive
                         | fs::copy_options::update_existing;
  fs::copy(codegen_src, dest_dir, copyOptions);

  // Make sure the path ends with a "/" (or a "\" for Windows)
  std::string c_src_dir = dest_src_dir.string();
  if (!c_src_dir.empty() && c_src_dir.back() != '/' && c_src_dir.back() != '\\')
      c_src_dir += fs::path::preferred_separator;

  CodeGen::TPGGenerationEngineFactory factory(CodeGen::TPGGenerationEngineFactory::switchMode);
	std::unique_ptr<CodeGen::TPGGenerationEngine> tpggen = factory.create("generated_tpg", *agent->getTPGGraph(), c_src_dir);
	tpggen->generateTPGGraph();
}
