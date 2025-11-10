#ifndef GYM_ENVIRONMENT_WRAPPER_H
#define GYM_ENVIRONMENT_WRAPPER_H

#include "data/arrayWrapper.h"
#include "learn/learningEnvironment.h"
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

class GymEnvironment : public Learn::LearningEnvironment {
public:
  GymEnvironment(py::args args, const py::kwargs &kwargs);
  GymEnvironment(const py::object &gymEnv, py::args args,
                 const py::kwargs &kwargs);
  GymEnvironment(const py::function &initEnvCallback, py::args args,
                 const py::kwargs &kwargs);
  Learn::LearningEnvironment *clone() const override;

  // Inherited from LearningEnvironment
  void reset(size_t seed = 0,
             Learn::LearningMode mode = Learn::LearningMode::TRAINING,
             uint16_t iterationNumber = 0,
             uint64_t generationNumber = 0) override;

  void doAction(double actionID) override;
  void doActions(std::vector<double> vectActionID) override;

  double getScore() const override;
  bool isTerminal() const override;
  void render();

  std::vector<std::reference_wrapper<const Data::DataHandler>>
  getDataSources() override;

private:
  std::unique_ptr<Data::ArrayWrapper<double>> observations;
  std::vector<double> obs;

  // Python Gym environment
  py::object gymEnv;
  py::function initEnvCallback;
  py::args envArgs;
  const py::kwargs &envKwargs;

  double reward; // Last reward
  bool done;     // Terminal state
};

#endif
