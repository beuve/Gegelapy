#include "algorithms/algorithm.h"
#include "instructions.h"
#include "trainer.h"
#include "learn/learningEnvironment.h"
#include "outputConversions.h"
#include <pybind11/pybind11.h>
#include <pybind11/pytypes.h>
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

  m.def("getOutputHandlerFromEnv", getOutputHandlerFromEnv);

  py::class_<Learn::LearningEnvironment>(m, "LearningEnvironment")
      .def("doAction", &Learn::LearningEnvironment::doAction)
      .def("doActions", &Learn::LearningEnvironment::doActions)
      .def("reset", &Learn::LearningEnvironment::reset)
      .def("getScore", &Learn::LearningEnvironment::getScore)
      .def("isTerminal", &Learn::LearningEnvironment::isTerminal);

  py::class_<GymEnvironment, Learn::LearningEnvironment>(m, "GymEnvironment")
      .def(py::init<py::function, py::args, py::kwargs &>())
      .def(py::init<py::args, py::kwargs &>())
      .def("reset", &GymEnvironment::reset)
      .def("step", &GymEnvironment::doAction)
      //.def("render", &ParGymEnvironment::render)
      .def("get_score", &GymEnvironment::getScore)
      .def("is_done", &GymEnvironment::isTerminal);

  py::class_<Trainer>(m, "Trainer")
      .def(py::init<GymEnvironment&, Algorithm::Algorithm&>())
      .def("train", &Trainer::train)
      .def("step", &Trainer::step);

  add_algorithms(m);
}
