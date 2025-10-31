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

  py::class_<Instructions::Instruction>(m, "Instruction");
  py::class_<Instructions::LambdaInstruction<double>, Instructions::Instruction>(m, "LambdaInstruction1");
  py::class_<Instructions::LambdaInstruction<double, double>, Instructions::Instruction>(m, "LambdaInstruction2");

  m.attr("add") = py::cast(i_add);
  m.attr("sub") = py::cast(i_sub);
  m.attr("div") = py::cast(i_div);
  m.attr("mul") = py::cast(i_mul);
  m.attr("exp") = py::cast(i_exp);
  m.attr("ln") = py::cast(i_ln);
  m.attr("max") = py::cast(i_max);
  m.attr("sin") = py::cast(i_sin);
  m.attr("cos") = py::cast(i_cos);
  m.attr("tan") = py::cast(i_tan);
  m.attr("pi") = py::cast(i_pi);

  py::class_<GymTrainer>(m, "TPG")
    .def(py::init<GymEnvironment&, const std::vector<Instructions::Instruction*>&>(),
         py::arg("env"),
         py::arg("instructions"))
    .def("train", &GymTrainer::train)
    .def("step", &GymTrainer::step)
    .def("save", &GymTrainer::save, py::arg("path"), py::arg("clean") = false)
    .def("load", &GymTrainer::load);
}
