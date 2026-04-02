#include "data/arrayWrapper.h"
#include "learn/learningEnvironment.h"
#include <pybind11/pybind11.h>
#include <vector>

namespace py = pybind11;

class GymEnvironment : public Learn::LearningEnvironment {
public:
    // Constructors matching your requirements
    GymEnvironment(const py::object &args, const py::kwargs &kwargs);
    GymEnvironment(const py::function &initEnvCallback, const py::object &args, const py::kwargs &kwargs);
    
    virtual ~GymEnvironment();
    Learn::LearningEnvironment *clone() const override;

    void reset(size_t seed = 0, Learn::LearningMode mode = Learn::LearningMode::TRAINING,
               uint16_t iter = 0, uint64_t gen = 0) override;

    void doAction(double actionID) override;
    void doActions(std::vector<double> vectActionID) override;

    double getScore() const override { return reward; }
    bool isTerminal() const override { return done; }
    std::vector<std::reference_wrapper<const Data::DataHandler>> getDataSources() override;

private:
    void spawnWorker(py::object payload);
    
    void* writeHandle;        
    void* readHandle;         
    void* childProcessHandle; 
    
    double reward;
    bool done;
    std::vector<double> obs;
    std::unique_ptr<Data::ArrayWrapper<double>> observations;

    // Cache for cloning
    py::object initEnvCallback;
    py::object envArgs;
    py::object envKwargs;
};
