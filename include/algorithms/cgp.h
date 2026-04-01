#include "algorithm/cgp/cgpAlgorithm.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

class PyCGP : public Algorithm::CGPAlgorithm {
public:
  PyCGP(const std::vector<Instructions::Instruction *> &instructions)
      : CGPAlgorithm(instructionSet) {
    py::gil_scoped_acquire acquire;
    // Build the instruction set from provided list
    for (auto *instr : instructions) {
      if (instr != nullptr) {
        instructionSet.add(*instr);
      } else {
        std::cerr << "[PyCGP] Warning: null instruction skipped." << std::endl;
      }
    }
  }

protected:
  Instructions::Set instructionSet;
};
