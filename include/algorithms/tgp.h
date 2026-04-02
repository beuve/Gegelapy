#include "algorithm/tgp/tgpAlgorithm.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

class PyTGP : public Algorithm::TGPAlgorithm {
public:
  PyTGP(const std::vector<Instructions::Instruction *> &instructions,
        size_t nbAgents, double maxConstValue, double minConstValue,
        size_t nbProgramConstant, double pConstantMutation, double pMutate,
        double pMutateOutput, size_t maxDepth, size_t maxInitDepth,
        size_t maxNbEdgePerNode)
      : TGPAlgorithm(instructionSet) {

    params->nbAgents = nbAgents;
    params->lgp.maxConstValue = maxConstValue;
    params->lgp.minConstValue = minConstValue;
    params->lgp.nbProgramConstant = nbProgramConstant;
    params->lgp.pConstantMutation = pConstantMutation;
    params->lgp.pMutate = pMutate;
    params->lgp.pMutateOutput = pMutateOutput;
    params->tgp.maxDepth = maxDepth;
    params->tgp.maxInitDepth = maxInitDepth;
    params->tgp.maxNbEdgePerNode = maxNbEdgePerNode;

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
