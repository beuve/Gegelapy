#include "selector/truncationSelector.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

class PyTruncation : public Selector::TruncationSelector {
public:
  PyTruncation(double ratioDeletedRoots = 0.5)
      : TruncationSelector(){
        params->truncation.ratioDeletedRoots = ratioDeletedRoots;
      }
    
private:
};
