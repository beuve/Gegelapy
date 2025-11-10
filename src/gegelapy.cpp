#include "gymEnvironment.h"
#include "instructions.h"
#include "learn/learningEnvironment.h"
#include "tpg.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#if (PYBIND11_VERSION_MAJOR > 2) ||                                            \
    (PYBIND11_VERSION_MAJOR == 2 && PYBIND11_VERSION_MINOR >= 13)
#define MODULE(name, m) PYBIND11_MODULE(name, m, py::mod_gil_not_used())
#else
#define MODULE(name, m) PYBIND11_MODULE(name, m)
#endif

MODULE(gegelapy, m) {
  m.doc() = "Python bindings for Gegelati with Gym integration";

  add_instructions(m);

  py::class_<Learn::LearningEnvironment>(m, "LearningEnvironment")
      .def("getNbActions", &Learn::LearningEnvironment::getNbActions)
      .def("isDiscrete", &Learn::LearningEnvironment::isDiscrete)
      .def("getInitActions", &Learn::LearningEnvironment::getInitActions)
      .def("doAction", &Learn::LearningEnvironment::doAction)
      .def("doActions", &Learn::LearningEnvironment::doActions)
      .def("reset", &Learn::LearningEnvironment::reset)
      .def("getScore", &Learn::LearningEnvironment::getScore)
      .def("isTerminal", &Learn::LearningEnvironment::isTerminal);

  py::class_<GymEnvironment, Learn::LearningEnvironment>(m, "GymEnvironment")
      .def(py::init<py::function, py::args, py::kwargs &>())
      .def(py::init<py::object, py::args, py::kwargs &>())
      .def(py::init<py::args, py::kwargs &>())
      .def("reset", &GymEnvironment::reset)
      .def("step", &GymEnvironment::doAction)
      .def("render", &GymEnvironment::render)
      .def("get_score", &GymEnvironment::getScore)
      .def("is_done", &GymEnvironment::isTerminal);

  add_tpg(m);
}
