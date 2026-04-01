#include "outputConversions.h"
#include <cstdio>
#include <pybind11/numpy.h>
#include <pybind11/pytypes.h>
#include <vector>

Output::OutputHandler convertFromDiscrete(py::object action_space) {
  return Output::OutputHandler(action_space.attr("n").cast<size_t>());
}

int getActionSpaceLength(py::object action_space) {
  py::tuple shape = action_space.attr("_shape");
  int res = 1;
  for (int i = 0; i < shape.size(); i++) {
    res *= shape[i].cast<int>();
  }
  return res;
}

Output::OutputHandler convertFromBox(py::object action_space) {
  int len = getActionSpaceLength(action_space);
  py::array_t<double> lows =
      action_space.attr("low").cast<py::array_t<double>>();
  py::array_t<double> highs =
      action_space.attr("high").cast<py::array_t<double>>();
  std::vector<Output::Output> outputs = std::vector<Output::Output>();
  for (int i = 0; i < len; i++) {
    outputs.push_back(Output::Output(lows.at(i), highs.at(i)));
  }
  return Output::OutputHandler(outputs);
}

Output::OutputHandler
getOutputHandlerFromEnv(const py::function &callback,
                        const py::args &args, const py::kwargs &kwargs) {
  py::module spaces = py::module::import("gymnasium").attr("spaces");
  py::object action_space = callback(*args, **kwargs).attr("action_space");
  if (py::isinstance(action_space, spaces.attr("Discrete"))) {
    return convertFromDiscrete(action_space);
  }
  return convertFromBox(action_space);
}
