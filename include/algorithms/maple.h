#include "algorithm/maple/mapleAlgorithm.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

class PyMaple : public Algorithm::MapleAlgorithm {
public:
  PyMaple(const Algorithm& programAlgorithm)
      : MapleAlgorithm(programAlgorithm){}
};
