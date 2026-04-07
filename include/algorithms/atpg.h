#include "algorithm/atpg/atpgAlgorithm.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

class PyATPG : public Algorithm::ATPGAlgorithm {
public:
  PyATPG(const Algorithm& contextProgramAlgorithm,const Algorithm& actionProgramAlgorithm, size_t nbAgents, size_t archiveSize,
        double archivingProbability,
        size_t maxInitOutgoingEdges, size_t maxOutgoingEdges,
        double pEdgeAddition, double pEdgeDeletion,
        double pEdgeDestinationChange, double pEdgeDestinationIsAction,
        double pProgramMutation, double probaContextOverActionProgram)
      : ATPGAlgorithm(contextProgramAlgorithm, actionProgramAlgorithm){
        params->nbAgents = nbAgents;
        params->tpg.archiveSize = archiveSize;
        params->tpg.archivingProbability = archivingProbability;
        params->tpg.maxInitOutgoingEdges = maxInitOutgoingEdges;
        params->tpg.maxOutgoingEdges = maxOutgoingEdges;
        params->tpg.pEdgeAddition = pEdgeAddition;
        params->tpg.pEdgeDeletion = pEdgeDeletion;
        params->tpg.pEdgeDestinationChange = pEdgeDestinationChange;
        params->tpg.pEdgeDestinationIsAction = pEdgeDestinationIsAction;
        params->tpg.pProgramMutation = pProgramMutation;
        params->atpg.probaContextOverActionProgram = probaContextOverActionProgram;
      }
};
