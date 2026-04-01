#include "algorithm/lgp/lgpAlgorithm.h"
#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

class PyLGP : public Algorithm::LGPAlgorithm {
public:
  PyLGP(const std::vector<Instructions::Instruction *> &instructions)
      : LGPAlgorithm(instructionSet) {
    py::gil_scoped_acquire acquire;
    // Build the instruction set from provided list
    for (auto *instr : instructions) {
      if (instr != nullptr) {
        instructionSet.add(*instr);
      } else {
        std::cerr << "[PyLGP] Warning: null instruction skipped." << std::endl;
      }
    }
  }

protected:
  Instructions::Set instructionSet;
};
