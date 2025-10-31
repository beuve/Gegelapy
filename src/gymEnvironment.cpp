#include "gymEnvironment.h"
#include <iostream>

GymEnvironment::GymEnvironment(const std::string &gymEnvName)
    : Learn::LearningEnvironment(py::module::import("gymnasium").attr("make")(gymEnvName).attr("action_space").attr("n").cast<uint64_t>(), true), reward(0.0), done(false) {
  py::gil_scoped_acquire acquire;
  py::module gym = py::module::import("gymnasium");
  gymEnv = gym.attr("make")(gymEnvName);
  reset();
  observations = std::make_unique<Data::ArrayWrapper<double>>(obs.size(), &obs);
}

GymEnvironment::GymEnvironment(const py::object &gymEnv)
    : Learn::LearningEnvironment(1, true), reward(0.0), done(false) {
  py::gil_scoped_acquire acquire;
  py::module gym = py::module::import("gymnasium");
  this->gymEnv = gymEnv;
  reset();
  observations = std::make_unique<Data::ArrayWrapper<double>>(obs.size(), &obs);
}

void GymEnvironment::reset(size_t seed, Learn::LearningMode mode,
                           uint16_t iterationNumber,
                           uint64_t generationNumber) {
  py::gil_scoped_acquire acquire;
  auto result = gymEnv.attr("reset")(py::arg("seed") = seed);
  py::tuple result_tuple = result.cast<py::tuple>();
  py::object py_obs = result_tuple[0];
  auto obs_array = py_obs.cast<py::array_t<double>>();
  for (ssize_t i = 0; i < obs_array.size(); ++i)
      obs.push_back(obs_array.at(i));
  reward = 0.0;
  done = false;
}

void GymEnvironment::doAction(double actionID) {
    py::gil_scoped_acquire acquire;

    py::object result = gymEnv.attr("step")(py::cast((int)actionID));
    py::tuple result_tuple = result.cast<py::tuple>();

    py::object py_obs = result_tuple[0];
    // TODO: Verify if it is always an array
    auto obs_array = py_obs.cast<py::array_t<double>>(); 
    for (ssize_t i = 0; i < obs_array.size(); ++i)
      obs[i] = obs_array.at(i);

    reward += result_tuple[1].cast<double>();
    done = result_tuple[2].cast<bool>() || result_tuple[3].cast<bool>();
}


void GymEnvironment::doActions(std::vector<double> vectActionID) {
  if (vectActionID.size() == 1)
    doAction(vectActionID[0]);
  else
    throw std::runtime_error("Multi-action not implemented for GymEnvironment");
}

void GymEnvironment::render() {
  py::gil_scoped_acquire acquire;
  gymEnv.attr("render")();
}

double GymEnvironment::getScore() const { return reward; }

bool GymEnvironment::isTerminal() const { return done; }

std::vector<std::reference_wrapper<const Data::DataHandler>>
GymEnvironment::getDataSources() {
  auto result = std::vector<std::reference_wrapper<const Data::DataHandler>>();
	result.push_back(*this->observations);
	return result;
}
