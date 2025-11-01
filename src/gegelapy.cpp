#include "gymEnvironment.h"
#include "instructions.h"
#include "tpg.h"
#include "instructions/instruction.h"
#include "instructions/lambdaInstruction.h"
#include "learn/learningAgent.h"
#include "learn/learningEnvironment.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

PYBIND11_MODULE(gegelapy, m) {
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
        .def(py::init<const std::string &>(), py::arg("gymEnvName"))
        .def("reset", &GymEnvironment::reset)
        .def("step", &GymEnvironment::doAction)
        .def("render", &GymEnvironment::render)
        .def("get_score", &GymEnvironment::getScore)
        .def("is_done", &GymEnvironment::isTerminal);

  add_tpg(m);
}
