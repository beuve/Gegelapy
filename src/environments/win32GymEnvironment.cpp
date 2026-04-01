#include "environments/gymEnvironment.h"
#include "outputConversions.h"
#include <windows.h>
#include <filesystem>
#include <stdexcept>

namespace fs = std::filesystem;

// Helper to handle Windows-specific cleanup and error throwing
void ThrowLastError(const std::string& msg) {
    throw std::runtime_error(msg + " (Error: " + std::to_string(GetLastError()) + ")");
}

GymEnvironment::GymEnvironment(const py::object &args, const py::kwargs &kwargs)
    : GymEnvironment(py::module::import("gymnasium").attr("make"), args, kwargs) {}

GymEnvironment::GymEnvironment(const py::function &callback, const py::object &args, const py::kwargs &kwargs)
    : Learn::LearningEnvironment(getOutputHandlerFromEnv(callback, args, kwargs)), 
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
    reset(); 
    observations = std::make_unique<Data::ArrayWrapper<double>>(obs.size(), &obs);
}

void GymEnvironment::spawnWorker(py::object payload) {
    HANDLE hChildStdIn_Rd = NULL;
    HANDLE hChildStdIn_Wr = NULL;
    HANDLE hChildStdOut_Rd = NULL;
    HANDLE hChildStdOut_Wr = NULL;

    SECURITY_ATTRIBUTES saAttr;
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE; // Handles must be inheritable
    saAttr.lpSecurityDescriptor = NULL;

    // Create pipes for Stdout and Stdin
    if (!CreatePipe(&hChildStdOut_Rd, &hChildStdOut_Wr, &saAttr, 0)) ThrowLastError("Stdout Pipe");
    if (!CreatePipe(&hChildStdIn_Rd, &hChildStdIn_Wr, &saAttr, 0)) ThrowLastError("Stdin Pipe");

    // Ensure the parent's end of the pipe is NOT inherited
    SetHandleInformation(hChildStdOut_Rd, HANDLE_FLAG_INHERIT, 0);
    SetHandleInformation(hChildStdIn_Wr, HANDLE_FLAG_INHERIT, 0);

    // Prepare paths
    py::object sys = py::module::import("sys");
    std::string pyExe = sys.attr("executable").cast<std::string>();
    py::object main_module = py::module_::import("gegelapy");
    std::string module_path = py::cast<std::string>(main_module.attr("__file__"));
    std::string worker_path = fs::path(module_path).parent_path().string() + "/gym_worker.py";
    
    // Windows requires a single command line string, not an array
    std::string cmdLine = "\"" + pyExe + "\" \"" + worker_path + "\"";

    PROCESS_INFORMATION piProcInfo;
    STARTUPINFOA siStartInfo; // Use ANSI version
    ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));
    ZeroMemory(&siStartInfo, sizeof(STARTUPINFOA));
    
    siStartInfo.cb = sizeof(STARTUPINFOA);
    siStartInfo.hStdError = hChildStdOut_Wr; // Redirect stderr to stdout pipe
    siStartInfo.hStdOutput = hChildStdOut_Wr;
    siStartInfo.hStdInput = hChildStdIn_Rd;
    siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

    if (!CreateProcessA(NULL, (LPSTR)cmdLine.c_str(), NULL, NULL, TRUE, 0, NULL, NULL, &siStartInfo, &piProcInfo)) {
        ThrowLastError("CreateProcess failed");
    }

    // Close child-side handles in parent
    CloseHandle(hChildStdOut_Wr);
    CloseHandle(hChildStdIn_Rd);
    CloseHandle(piProcInfo.hThread); // We don't need the thread handle

    // Store handles (assuming you changed writeFd/readFd to HANDLE in the header)
    this->writeHandle = hChildStdIn_Wr;
    this->readHandle = hChildStdOut_Rd;
    this->childProcessHandle = piProcInfo.hProcess;

    // Send the pickled environment configuration
    py::object pickle = py::module::import("pickle");
    std::string serialized = pickle.attr("dumps")(payload).cast<py::bytes>();
    uint32_t size = (uint32_t)serialized.size();
    
    DWORD dwWritten;
    WriteFile(writeHandle, &size, 4, &dwWritten, NULL);
    WriteFile(writeHandle, serialized.data(), size, &dwWritten, NULL);
}

void GymEnvironment::doActions(std::vector<double> vectActionID) {
    char cmd = 's';
    DWORD dwWritten, dwRead;
    WriteFile(writeHandle, &cmd, 1, &dwWritten, NULL);
    
    uint32_t numActions = static_cast<uint32_t>(vectActionID.size());
    WriteFile(writeHandle, &numActions, 4, &dwWritten, NULL);
    WriteFile(writeHandle, vectActionID.data(), numActions * sizeof(double), &dwWritten, NULL);

    struct { double r; bool d; uint32_t sz; } header;
    ReadFile(readHandle, &header, sizeof(header), &dwRead, NULL);
    
    if (obs.size() == header.sz) {
        ReadFile(readHandle, obs.data(), header.sz * sizeof(double), &dwRead, NULL);
    } else {
        throw std::runtime_error("Observation size mismatch!");
    }

    reward += header.r;
    done = header.d;
}

void GymEnvironment::reset(size_t seed, Learn::LearningMode, uint16_t, uint64_t) {
    char cmd = 'r';
    DWORD dwWritten, dwRead;
    WriteFile(writeHandle, &cmd, 1, &dwWritten, NULL);
    
    uint64_t s = seed;
    WriteFile(writeHandle, &s, sizeof(uint64_t), &dwWritten, NULL);

    uint32_t sz;
    ReadFile(readHandle, &sz, 4, &dwRead, NULL);
    obs.resize(sz);
    ReadFile(readHandle, obs.data(), sz * sizeof(double), &dwRead, NULL);

    reward = 0.0;
    done = false;
}

GymEnvironment::~GymEnvironment() {
    if (writeHandle) CloseHandle(writeHandle);
    if (readHandle) CloseHandle(readHandle);

    if (childProcessHandle) {
        TerminateProcess(childProcessHandle, 0); // Equivalent to SIGKILL
        CloseHandle(childProcessHandle);
    }
    // GIL cleanup...
}
