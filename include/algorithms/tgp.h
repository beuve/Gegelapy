#include "algorithm/tgp/tgpAlgorithm.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

class PyTGP : public Algorithm::TGPAlgorithm {
public:
  PyTGP(const std::vector<Instructions::Instruction *> &instructions)
      : TGPAlgorithm(instructionSet) {
    py::gil_scoped_acquire acquire;
    // Build the instruction set from provided list
    for (auto *instr : instructions) {
      if (instr != nullptr) {
        instructionSet.add(*instr);
      } else {
        std::cerr << "[PyTGP] Warning: null instruction skipped." << std::endl;
      }
    }
  }

protected:
  Instructions::Set instructionSet;
};
