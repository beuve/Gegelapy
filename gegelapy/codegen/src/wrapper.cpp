#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

// Declare the symbols from generated code
extern "C" {
    extern double* in1 = nullptr;
    double inferenceTPG();
}

namespace py = pybind11;

py::object infer(py::array_t<double> input) {
    auto buf = input.request();
    if (buf.ndim != 1)
        throw std::runtime_error("Input must be a 1D numpy array");

    // Assign global pointer
    in1 = static_cast<double*>(buf.ptr);

    // Run inference
    double result = inferenceTPG();
    py::object py_result = py::float_(result);
    return py_result.cast<py::int_>();
}

PYBIND11_MODULE(gen_tpg, m) {
    m.def("infer", &infer, "Run inference on the generated TPG");
}
