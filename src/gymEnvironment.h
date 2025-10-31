#ifndef GYM_ENVIRONMENT_WRAPPER_H
#define GYM_ENVIRONMENT_WRAPPER_H

#include "learn/learningEnvironment.h"
#include "data/arrayWrapper.h"
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

class GymEnvironment : public Learn::LearningEnvironment {
public:
  GymEnvironment(const std::string &gymEnvName);
  GymEnvironment(const py::object &gymEnv);

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
  py::object gymEnv;      // Python Gym environment
  std::unique_ptr<Data::ArrayWrapper<double>> observations;
  std::vector<double> obs;
  double reward;          // Last reward
  bool done;              // Terminal state
};

#endif
