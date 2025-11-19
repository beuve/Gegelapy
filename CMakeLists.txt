cmake_minimum_required(VERSION 3.14)
project(Gegelapy LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Add Gegelati submodule
set(BUILD_TESTING OFF CACHE BOOL "Disable GEGELATI tests" FORCE)
set(SKIP_DOXYGEN_BUILD ON CACHE BOOL "Disable GEGELATI doxygen" FORCE)

add_subdirectory(extern/gegelati)
add_subdirectory(extern/pybind11)

# Create Python module
pybind11_add_module(gegelapy
    src/gegelapy.cpp
    src/gymEnvironment.cpp
    src/gymTrainer.cpp
    src/instructions.cpp
    src/tpg.cpp
)

# Include Gegelati headers
target_include_directories(gegelapy PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/gegelati/gegelatilib/include
)

# Link Gegelati library and pybind11
target_link_libraries(gegelapy PRIVATE GEGELATI)

# Where we want the built libraries to go:
set(GEGELAPY_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/gegelapy)

# Place the compiled pybind module in gegelapy/
set_target_properties(gegelapy PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY $<1:${GEGELAPY_OUTPUT_DIR}>
    RUNTIME_OUTPUT_DIRECTORY $<1:${GEGELAPY_OUTPUT_DIR}>
)

# Place the Gegelati shared library in gegelapy/
set_target_properties(GEGELATI PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY $<1:${GEGELAPY_OUTPUT_DIR}>
    RUNTIME_OUTPUT_DIRECTORY $<1:${GEGELAPY_OUTPUT_DIR}>
)

install(TARGETS gegelapy DESTINATION gegelapy)

install(TARGETS GEGELATI DESTINATION gegelapy)
