#include "selectors/selectors.h"
#include "selector/selector.h"
#include "selectors/tournament.h"
#include "selectors/truncation.h"
#include <cstdint>
#include <pybind11/cast.h>
#include <pybind11/pytypes.h>

void add_selectors(py::module_ m) {
  auto sel =
      py::class_<Selector::Selector, std::shared_ptr<Selector::Selector>>(
          m, "Selector");

  auto trunament = py::class_<PyTournament, Selector::Selector, std::shared_ptr<PyTournament>>(m, "Tournament");
  trunament.def(py::init<bool, double, uint64_t>(), py::kw_only(),
                py::arg("are_elites_reproductible") = true,
                py::arg("ratio_saved_roots") = 0.05,
                py::arg("size_tournament") = 3);

  auto truncation = py::class_<PyTruncation, Selector::Selector, std::shared_ptr<PyTruncation>>(m, "Truncation");
  truncation.def(py::init<double>(), py::kw_only(),
                 py::arg("ratio_deleted_roots") = 0.5);
}
