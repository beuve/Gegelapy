#include "algorithm/algorithm.h"
#include "algorithms/algorithm.h"
#include "algorithms/atpg.h"
#include "algorithms/lgp.h"
#include "algorithms/maple.h"
#include "algorithms/tpg.h"
#include "algorithms/tgp.h"
#include "algorithms/cgp.h"
#include <pybind11/pytypes.h>
#include <filesystem>

void save_algorithm(Algorithm::Algorithm &self, const char *path, bool clean) {
  if (clean) {
    self.clearUnusedAgentParts();
  }
  self.exportDotFile(path);
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
  cl.def("load", load_algorithm);
  cl.def("compile", compile_algorithm);
}

void add_algorithms(py::module_ m) {
  auto alg = py::class_<Algorithm::Algorithm>(m, "Algorithm");

  auto lgp = py::class_<PyLGP, Algorithm::Algorithm>(m, "LGP");
  lgp.def(py::init<const std::vector<Instructions::Instruction *> &>());
  register_common_methods(lgp);

  auto cgp = py::class_<PyCGP, Algorithm::Algorithm>(m, "CGP");
  cgp.def(py::init<const std::vector<Instructions::Instruction *> &>());
  register_common_methods(cgp);

  auto tgp = py::class_<PyTGP, Algorithm::Algorithm>(m, "TGP");
  tgp.def(py::init<const std::vector<Instructions::Instruction *> &>());
  register_common_methods(tgp);

  auto tpg = py::class_<PyTPG, Algorithm::Algorithm>(m, "TPG");
  tpg.def(py::init<const Algorithm::Algorithm &>());
  register_common_methods(tpg);

  auto maple = py::class_<PyMaple, Algorithm::Algorithm>(m, "Maple");
  maple.def(py::init<const Algorithm::Algorithm &>());
  register_common_methods(maple);

  auto atpg = py::class_<PyATPG, Algorithm::Algorithm>(m, "ATPG");
  atpg.def(
      py::init<const Algorithm::Algorithm &, const Algorithm::Algorithm &>());
  register_common_methods(atpg);
}
