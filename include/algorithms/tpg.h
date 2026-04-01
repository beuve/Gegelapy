#include "algorithm/tpg/tpgAlgorithm.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

class PyTPG : public Algorithm::TPGAlgorithm {
public:
  PyTPG(const Algorithm& programAlgorithm)
      : TPGAlgorithm(programAlgorithm){}
};
