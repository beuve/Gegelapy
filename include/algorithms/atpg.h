#include "algorithm/atpg/atpgAlgorithm.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

class PyATPG : public Algorithm::ATPGAlgorithm {
public:
  PyATPG(const Algorithm& contextProgramAlgorithm,const Algorithm& actionProgramAlgorithm)
      : ATPGAlgorithm(contextProgramAlgorithm, actionProgramAlgorithm){}
};
