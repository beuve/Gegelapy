#include "algorithm/maple/mapleAlgorithm.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

class PyMaple : public Algorithm::MapleAlgorithm {
public:
  PyMaple(const Algorithm &programAlgorithm, size_t nbAgents,
          size_t archiveSize, double archivingProbability,
          bool forceProgramBehaviorChangeOnMutation, size_t nbActionEdgeInit,
          double pActionEdgeAddition, double pActionEdgeDeletion,
          double pChangeActionClass, double pCrossAgents, double pCrossPrograms,
          double pMutateActionProgram, double pSwapActionProgram)
      : MapleAlgorithm(programAlgorithm) {
    params->nbAgents = nbAgents;
    params->tpg.archiveSize = archiveSize;
    params->tpg.archivingProbability = archivingProbability;
    params->tpg.forceProgramBehaviorChangeOnMutation =
        forceProgramBehaviorChangeOnMutation;
    params->maple.nbActionEdgeInit = nbActionEdgeInit;
    params->maple.pActionEdgeAddition = pActionEdgeAddition;
    params->maple.pActionEdgeDeletion = pActionEdgeDeletion;
    params->maple.pChangeActionClass = pChangeActionClass;
    params->maple.pCrossAgents = pCrossAgents;
    params->maple.pCrossPrograms = pCrossPrograms;
    params->maple.pMutateActionProgram = pMutateActionProgram;
    params->maple.pSwapActionProgram = pSwapActionProgram;
  }
};
