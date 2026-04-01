#include "environments/gymEnvironment.h"
#include "outputConversions.h"
#include <cstddef>
#include <cstdio>
#include <signal.h>
#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>
#include <filesystem>

extern char **environ;
namespace fs = std::filesystem;

GymEnvironment::GymEnvironment(const py::object &args,
                                     const py::kwargs &kwargs)
    : GymEnvironment(py::module::import("gymnasium").attr("make"), args,
                        kwargs) {}

GymEnvironment::GymEnvironment(const py::function &callback,
                                     const py::object &args,
                                     const py::kwargs &kwargs)
    : Learn::LearningEnvironment(getOutputHandlerFromEnv(
          callback, args,
          kwargs)), // Initialize properly based on your OutputHandler
      reward(0.0), done(false) {
  py::gil_scoped_acquire acquire;
  this->initEnvCallback = callback;
  this->envArgs = args;
  this->envKwargs = kwargs;
  py::dict payload;
  payload["callback"] = callback;
  payload["args"] = args;
  payload["kwargs"] = kwargs;
  spawnWorker(payload);
  reset(); // Initial sync
  observations = std::make_unique<Data::ArrayWrapper<double>>(obs.size(), &obs);
}

void GymEnvironment::spawnWorker(py::object payload) {
  // py::gil_scoped_acquire acquire;
  int pToC[2], pFromC[2];
  pipe(pToC);
  pipe(pFromC);

  posix_spawn_file_actions_t actions;
  posix_spawn_file_actions_init(&actions);
  posix_spawn_file_actions_adddup2(&actions, pToC[0], STDIN_FILENO);
  posix_spawn_file_actions_adddup2(&actions, pFromC[1], STDOUT_FILENO);

  py::object sys = py::module::import("sys");
  std::string pyExe = sys.attr("executable").cast<std::string>();

  // Launch the external worker script
  py::object main_module = py::module_::import("gegelapy");
  std::string module_path = py::cast<std::string>(main_module.attr("__file__"));
  std::string worker_path = fs::path(module_path).parent_path().string() + "/gym_worker.py";
  const char *args[] = {pyExe.c_str(), worker_path.c_str(), nullptr};
  posix_spawn(&childPid, pyExe.c_str(), &actions, nullptr, (char *const *)args,
              environ);

  writeFd = pToC[1];
  readFd = pFromC[0];
  close(pToC[0]);
  close(pFromC[1]);
  posix_spawn_file_actions_destroy(&actions);

  // Send the pickled environment configuration
  py::object pickle = py::module::import("pickle");
  std::string serialized = pickle.attr("dumps")(payload).cast<py::bytes>();
  uint32_t size = serialized.size();
  write(writeFd, &size, 4);
  write(writeFd, serialized.data(), size);
}

void GymEnvironment::doAction(double actionID) { doActions({actionID}); }

void GymEnvironment::doActions(std::vector<double> vectActionID) {
  char cmd = 's';
  write(writeFd, &cmd, sizeof(char));
  uint32_t numActions = static_cast<uint32_t>(vectActionID.size());
  write(writeFd, &numActions, sizeof(uint32_t));
  write(writeFd, vectActionID.data(), numActions * sizeof(double));

  struct {
    double r;
    bool d;
    uint32_t sz;
  } header;
  read(readFd, &header, sizeof(header));
  if (obs.size() == header.sz) {
    read(readFd, obs.data(), header.sz * sizeof(double));
  } else {
    throw std::runtime_error("Observation size mismatch!");
  }

  reward += header.r;
  done = header.d;
}

void GymEnvironment::reset(size_t seed, Learn::LearningMode, uint16_t,
                              uint64_t) {
  char cmd = 'r';
  write(writeFd, &cmd, sizeof(char));
  uint64_t s = seed;
  write(writeFd, &s, sizeof(uint64_t));

  uint32_t sz;
  read(readFd, &sz, sizeof(uint32_t));
  obs.resize(sz);
  read(readFd, obs.data(), sz * sizeof(double));

  reward = 0.0;
  done = false;
}

Learn::LearningEnvironment *GymEnvironment::clone() const {
  py::gil_scoped_acquire acquire;

  auto *newEnv = new GymEnvironment(this->initEnvCallback, this->envArgs,
                                       this->envKwargs);
  newEnv->observations =
      std::make_unique<Data::ArrayWrapper<double>>(*this->observations);
  newEnv->observations->setPointer(&newEnv->obs);
  return newEnv;
}

GymEnvironment::~GymEnvironment() {
  if (writeFd > 0)
    close(writeFd);
  if (readFd > 0)
    close(readFd);

  if (childPid > 0) {
    kill(childPid, SIGKILL); // Use SIGKILL for workers to ensure instant death
    waitpid(childPid, nullptr, WNOHANG); // Non-blocking wait
  }
  {
    py::gil_scoped_acquire acquire;
    this->initEnvCallback = py::object();
    this->envArgs = py::object();
    this->envKwargs = py::object();
  }
}

std::vector<std::reference_wrapper<const Data::DataHandler>>
GymEnvironment::getDataSources() {
  auto result = std::vector<std::reference_wrapper<const Data::DataHandler>>();
  result.push_back(*this->observations);
  return result;
}
