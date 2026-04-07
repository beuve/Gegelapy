#include "algorithm/cgp/cgpAlgorithm.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

class PyCGP : public Algorithm::CGPAlgorithm {
public:
  PyCGP(const std::vector<Instructions::Instruction *> &instructions,size_t nbAgents,
        double maxConstValue, double minConstValue,
        size_t nbProgramConstant,
        double pConstantMutation,
        double pMutateOutput, size_t nbLayers, size_t nbNodesPerLayer, double pMutateNode, bool forceProgramBehaviorChangeOnMutation)
      : CGPAlgorithm(instructionSet) {

    params->nbAgents = nbAgents;
    params->lgp.maxConstValue = maxConstValue;
    params->lgp.minConstValue = minConstValue;
    params->lgp.nbProgramConstant = nbProgramConstant;
    params->lgp.pConstantMutation = pConstantMutation;
    params->lgp.pMutateOutput = pMutateOutput;
    params->cgp.nbLayers = nbLayers;
    params->cgp.nbNodesPerLayer = nbNodesPerLayer;
    params->cgp.pMutateNode = pMutateNode;
    params->lgp.forceProgramBehaviorChangeOnMutation = forceProgramBehaviorChangeOnMutation;

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
