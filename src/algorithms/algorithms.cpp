#include "algorithms/algorithms.h"
#include "algorithm/algorithm.h"
#include "algorithms/atpg.h"
#include "algorithms/cgp.h"
#include "algorithms/lgp.h"
#include "algorithms/maple.h"
#include "algorithms/tgp.h"
#include "algorithms/tpg.h"
#include <filesystem>
#include <pybind11/cast.h>
#include <pybind11/pytypes.h>

void save_algorithm(Algorithm::Algorithm &self, const char *path, bool clean) {
  if (clean) {
    self.clearUnusedAgentParts();
  }
  self.exportDotFile(path);
}

void save_best_algorithm(Algorithm::Algorithm &self, const char *path) {
  self.exportBestAgentDotFile(path);
}

void load_algorithm(Algorithm::Algorithm &self, const char *path) {
  self.importDotFile(path);
}

void compile_algorithm(Algorithm::Algorithm &self, const char *path) {
  namespace fs = std::filesystem;

  fs::path dest_dir(path);
  fs::path dest_src_dir = dest_dir / "src";

  // Get gegelapy installation path to retrieve the codegen folder
  py::gil_scoped_acquire acquire;
  py::object gegelapy_module = py::module::import("gegelapy");
  fs::path gegelapy_path = gegelapy_module.attr("__file__").cast<std::string>();
  std::filesystem::path codegen_src =
      fs::path(gegelapy_path).parent_path() / "codegen";

  // Copy the codegen folder to the destination path
  const auto copyOptions =
      fs::copy_options::recursive | fs::copy_options::update_existing;
  fs::copy(codegen_src, dest_dir, copyOptions);

  // Make sure the path ends with a "/" (or a "\" for Windows)
  std::string c_src_dir = dest_src_dir.string();
  if (!c_src_dir.empty() && c_src_dir.back() != '/' && c_src_dir.back() != '\\')
    c_src_dir += fs::path::preferred_separator;

  self.exportBestAgentCodeGen("gen_", c_src_dir);
}

template <typename PyClass> void register_common_methods(PyClass &cl) {
  cl.def("save", save_algorithm, py::arg("path"), py::arg("clean") = false);
  cl.def("save_best", save_best_algorithm);
  cl.def("load", load_algorithm);
  cl.def("compile", compile_algorithm);
}

void add_algorithms(py::module_ m) {
  auto alg = py::class_<Algorithm::Algorithm>(m, "Algorithm");

  auto lgp = py::class_<PyLGP, Algorithm::Algorithm>(m, "LGP");
  lgp.def(py::init<const std::vector<Instructions::Instruction *> &, size_t,
                   size_t, size_t, double, double, size_t, size_t, size_t,
                   double, double, double, double, double, double, double, bool>(),
          py::arg("instructions"), py::kw_only(), py::arg("nb_agents") = 100,
          py::arg("init_max_program_size") = 10,
          py::arg("init_min_program_size") = 1, py::arg("maxConstValue") = 1.0,
          py::arg("minConstValue") = -1.0, py::arg("maxProgramSize") = 40,
          py::arg("nbProgramConstant") = 0, py::arg("nbRegisters") = 8,
          py::arg("pAdd") = 0.5, py::arg("pDelete") = 0.5,
          py::arg("pConstantMutation") = 0.4, py::arg("pMutate") = 1.0,
          py::arg("pMutateOutput") = 0.5, py::arg("pNewProgram") = 0.0,
          py::arg("pSwap") = 1.0, py::arg("forceProgramBehaviorChangeOnMutation") = false);
  register_common_methods(lgp);

  auto cgp = py::class_<PyCGP, Algorithm::Algorithm>(m, "CGP");
  cgp.def(
      py::init<const std::vector<Instructions::Instruction *> &, size_t, double,
               double, size_t, double, double, size_t, size_t, double, bool>(),
      py::arg("instructions"), py::kw_only(), py::arg("nb_agents") = 100,
      py::arg("maxConstValue") = 1.0, py::arg("minConstValue") = -1.0,
      py::arg("nbProgramConstant") = 0, py::arg("pConstantMutation") = 0.0,
      py::arg("pMutateOutput") = 0.5, py::arg("nbLayers") = 10,
      py::arg("nbNodesPerLayer") = 3, py::arg("pMutateNode") = 0.1, py::arg("forceProgramBehaviorChangeOnMutation") = false);
  register_common_methods(cgp);

  auto tgp = py::class_<PyTGP, Algorithm::Algorithm>(m, "TGP");
  tgp.def(py::init<const std::vector<Instructions::Instruction *> &, size_t,
                   double, double, size_t, double, double, double, size_t,
                   size_t, size_t, bool>(),
          py::arg("instructions"), py::kw_only(), py::arg("nb_agents") = 100,
          py::arg("maxConstValue") = 1.0, py::arg("minConstValue") = -1.0,
          py::arg("nbProgramConstant") = 0, py::arg("pConstantMutation") = 0.0,
          py::arg("pMutate") = 1.0, py::arg("pMutateOutput") = 0.5,
          py::arg("maxDepth") = 5, py::arg("maxInitDepth") = 3,
          py::arg("maxNbEdgePerNode") = 2, py::arg("forceProgramBehaviorChangeOnMutation") = false);
  register_common_methods(tgp);

  auto tpg = py::class_<PyTPG, Algorithm::Algorithm>(m, "TPG");
  tpg.def(py::init<const Algorithm::Algorithm &, size_t, size_t, double,
                   size_t, size_t, double, double, double, double, double>(),
          py::arg("programAlgorithm"), py::kw_only(), py::arg("nb_agents") = 100,
          py::arg("archiveSize") = 50, py::arg("archivingProbability") = 0.05,
          py::arg("maxInitOutgoingEdges") = 2, py::arg("maxOutgoingEdges") = 30,
          py::arg("pEdgeAddition") = 0.7, py::arg("pEdgeDeletion") = 0.7,
          py::arg("pEdgeDestinationChange") = 0.1,
          py::arg("pEdgeDestinationIsAction") = 0.9,
          py::arg("pProgramMutation") = 0.2);
  register_common_methods(tpg);

  auto maple = py::class_<PyMaple, Algorithm::Algorithm>(m, "Maple");
  maple.def(py::init<const Algorithm::Algorithm &, size_t, size_t, double,
                     size_t, double, double, double, double, double, double,
                     double>(),
            py::arg("programAlgorithm"), py::kw_only(),
            py::arg("nb_agents") = 100, py::arg("archiveSize") = 50,
            py::arg("archivingProbability") = 0.05,
            py::arg("nbActionEdgeInit") = 0, py::arg("pActionEdgeAddition") = 0.0,
            py::arg("pActionEdgeDeletion") = 0.0, py::arg("pChangeActionClass") = 0.1,
            py::arg("pCrossAgents") = 0.7, py::arg("pCrossPrograms") = 0.5,
            py::arg("pMutateActionProgram") = 0.7, py::arg("pSwapActionProgram") = 0.5);
  register_common_methods(maple);

  auto atpg = py::class_<PyATPG, Algorithm::Algorithm>(m, "ATPG");
  atpg.def(
      py::init<const Algorithm::Algorithm &, const Algorithm::Algorithm &, size_t, size_t, double,
                   size_t, size_t, double, double, double, double, double, double>(),
          py::arg("contextProgramAlgorithm"), py::arg("actionProgramAlgorithm"), py::kw_only(), py::arg("nb_agents") = 100,
          py::arg("archiveSize") = 50, py::arg("archivingProbability") = 0.05,
          py::arg("maxInitOutgoingEdges") = 3, py::arg("maxOutgoingEdges") = 30,
          py::arg("pEdgeAddition") = 0.7, py::arg("pEdgeDeletion") = 0.7,
          py::arg("pEdgeDestinationChange") = 0.1,
          py::arg("pEdgeDestinationIsAction") = 0.9,
          py::arg("pProgramMutation") = 0.2,
          py::arg("probaContextOverActionProgram") = 0.5);
  register_common_methods(atpg);
}
