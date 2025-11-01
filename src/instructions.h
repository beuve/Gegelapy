#include "instructions/set.h"
#include "instructions/lambdaInstruction.h"
#include "data/constant.h"
#include <cmath>
#include <algorithm>

namespace py = pybind11;

void add_instructions(py::module_ m) {

  py::class_<Instructions::Instruction>(m, "Instruction");
  py::class_<Instructions::LambdaInstruction<double>, Instructions::Instruction>(m, "LambdaInstruction1");
  py::class_<Instructions::LambdaInstruction<double, double>, Instructions::Instruction>(m, "LambdaInstruction2");

  auto f_add = [](double a, double b) -> double { return a + b; };
  auto i_add = Instructions::LambdaInstruction<double,double>(f_add, "$0 = $1 + $2;");
  m.attr("add") = py::cast(i_add);

  auto f_sub = [](double a, double b) -> double { return a - b; };
  auto i_sub = Instructions::LambdaInstruction<double,double>(f_sub, "$0 = $1 - $2;");
  m.attr("sub") = py::cast(i_sub);

  auto f_mul = [](double a, double b) -> double { return a * b; };
  auto i_mul = Instructions::LambdaInstruction<double,double>(f_mul, "$0 = $1 * $2;");
  m.attr("mul") = py::cast(i_mul);

  auto f_div = [](double a, double b) -> double { return a / b; };
  auto i_div = Instructions::LambdaInstruction<double,double>(f_div, "$0 = $1 / $2;");
  m.attr("div") = py::cast(i_div);

  auto f_max = [](double a, double b) -> double { return std::max(a, b); };
  auto i_max = Instructions::LambdaInstruction<double, double>(f_max, "$0 = (($1) < ($2)) ? ($2) : ($1);");
  m.attr("max") = py::cast(i_max);

  auto f_min = [](double a, double b) -> double { return std::min(a, b); };
  auto i_min = Instructions::LambdaInstruction<double, double>(f_min, "$0 = (($1) < ($2)) ? ($1) : ($2);");
  m.attr("min") = py::cast(i_min);

  auto f_ln = [](double a) -> double { return std::log(a); };
  auto i_ln = Instructions::LambdaInstruction<double>(f_ln, "$0 = log($1);");
  m.attr("ln") = py::cast(i_ln);

  auto f_exp = [](double a) -> double { return std::exp(a); };
  auto i_exp = Instructions::LambdaInstruction<double>(f_exp, "$0 = exp($1);");
  m.attr("exp") = py::cast(i_exp);

  auto f_cos = [](double a) -> double { return std::cos(a); };
  auto i_cos = Instructions::LambdaInstruction<double>(f_cos, "$0 = cos($1);");
  m.attr("cos") = py::cast(i_cos);

  auto f_sin = [](double a) -> double { return std::sin(a); };
  auto i_sin = Instructions::LambdaInstruction<double>(f_sin, "$0 = sin($1);");
  m.attr("sin") = py::cast(i_sin);

  auto f_tan = [](double a) -> double { return std::tan(a); };
  auto i_tan = Instructions::LambdaInstruction<double>(f_tan, "$0 = tan($1);");
  m.attr("tan") = py::cast(i_tan);

  auto f_pi = [](double) -> double { return M_PI; };
  auto i_pi = Instructions::LambdaInstruction<double>(f_pi, "$0 = M_PI;");
  m.attr("pi") = py::cast(i_pi);
}
