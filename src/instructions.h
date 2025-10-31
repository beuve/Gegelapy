#include "instructions/set.h"
#include "instructions/lambdaInstruction.h"
#include "data/constant.h"
#include <cmath>
#include <algorithm>

// Lambdas
auto f_add = [](double a, double b) -> double { return a + b; };
auto f_sub = [](double a, double b) -> double { return a - b; };
auto f_mul = [](double a, double b) -> double { return a * b; };
auto f_div = [](double a, double b) -> double { return a / b; };
auto f_max = [](double a, double b) -> double { return std::max(a, b); };
auto f_ln = [](double a) -> double { return std::log(a); };
auto f_exp = [](double a) -> double { return std::exp(a); };
auto f_cos = [](double a) -> double { return std::cos(a); };
auto f_sin = [](double a) -> double { return std::sin(a); };
auto f_tan = [](double a) -> double { return std::tan(a); };
auto f_pi = [](double) -> double { return M_PI; };
auto f_multByConst = [](double a, Data::Constant c) -> double { return a * (double)c / 10.0; };

// LambdaInstruction instances
auto i_add = Instructions::LambdaInstruction<double,double>(f_add, "$0 = $1 + $2;");
auto i_sub = Instructions::LambdaInstruction<double,double>(f_sub, "$0 = $1 - $2;");
auto i_mul = Instructions::LambdaInstruction<double,double>(f_mul, "$0 = $1 * $2;");
auto i_div = Instructions::LambdaInstruction<double,double>(f_div, "$0 = $1 / $2;");
auto i_max = Instructions::LambdaInstruction<double, double>(f_max, "$0 = (($1) < ($2)) ? ($2) : ($1);");
auto i_exp = Instructions::LambdaInstruction<double>(f_exp, "$0 = exp($1);");
auto i_ln = Instructions::LambdaInstruction<double>(f_ln, "$0 = log($1);");
auto i_cos = Instructions::LambdaInstruction<double>(f_cos, "$0 = cos($1);");
auto i_sin = Instructions::LambdaInstruction<double>(f_sin, "$0 = sin($1);");
auto i_tan = Instructions::LambdaInstruction<double>(f_tan, "$0 = tan($1);");
auto i_pi = Instructions::LambdaInstruction<double>(f_pi, "$0 = M_PI;");
auto i_multByConst = Instructions::LambdaInstruction<double, Data::Constant>(f_multByConst, "$0 = $1 * ((double)($2) / 10.0);");
