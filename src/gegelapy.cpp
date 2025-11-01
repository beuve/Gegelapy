#include "gymEnvironment.h"
#include "instructions.h"
#include "gymTrainer.h"
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

  py::class_<GymTrainer>(m, "TPG")
    .def(py::init<GymEnvironment&, const std::vector<Instructions::Instruction*>&>(),
         py::arg("env"),
         py::arg("instructions"))
    .def("train", &GymTrainer::train)
    .def("step", &GymTrainer::step)
    .def("save", &GymTrainer::save, py::arg("path"), py::arg("clean") = false)
    .def("load", &GymTrainer::load);
}
