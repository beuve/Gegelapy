#include "outputInfo.h"
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>

namespace py = pybind11;

Output::OutputHandler
getOutputHandlerFromEnv(const py::function &callback,
                        const py::args &args, const py::kwargs &kwargs);
