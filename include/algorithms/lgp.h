#include "algorithm/lgp/lgpAlgorithm.h"
#include <cstdint>
#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

class PyLGP : public Algorithm::LGPAlgorithm {
public:
  PyLGP(const std::vector<Instructions::Instruction *> &instructions,
        size_t nbAgents, size_t initMaxProgramSize, size_t initMinProgramSize,
        double maxConstValue, double minConstValue, size_t maxProgramSize,
        size_t nbProgramConstant, size_t nbRegisters, double pAdd,
        double pDelete, double pConstantMutation, double pMutate,
        double pMutateOutput, double pNewProgram, double pSwap)
      : LGPAlgorithm(instructionSet) {

    params->nbAgents = nbAgents;
    params->lgp.initMaxProgramSize = initMaxProgramSize;
    params->lgp.initMinProgramSize = initMinProgramSize;
    params->lgp.maxConstValue = maxConstValue;
    params->lgp.minConstValue = minConstValue;
    params->lgp.maxProgramSize = maxProgramSize;
    params->lgp.nbProgramConstant = nbProgramConstant;
    params->lgp.nbRegisters = nbRegisters;
    params->lgp.pAdd = pAdd;
    params->lgp.pDelete = pDelete;
    params->lgp.pConstantMutation = pConstantMutation;
    params->lgp.pMutate = pMutate;
    params->lgp.pMutateOutput = pMutateOutput;
    params->lgp.pNewProgram = pNewProgram;
    params->lgp.pSwap = pSwap;

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
