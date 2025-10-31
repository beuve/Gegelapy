cmake_minimum_required(VERSION 3.14)
project(Gegelapy LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Add Gegelati submodule
set(BUILD_TESTING OFF CACHE BOOL "Disable GEGELATI tests" FORCE)
set(SKIP_DOXYGEN_BUILD ON CACHE BOOL "Disable GEGELATI doxygen" FORCE)
add_subdirectory(gegelati)

# Find pybind11 installed
find_package(pybind11 REQUIRED)

# Create Python module
add_library(gegelapy MODULE
    src/gegelapy.cpp
    src/gymEnvironment.cpp
    src/gymTrainer.cpp
)

# Include Gegelati headers
target_include_directories(gegelapy PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/gegelati/gegelatilib/include
)

# Link Gegelati library and pybind11
target_link_libraries(gegelapy PRIVATE GEGELATI pybind11::module)

# Remove "lib" prefix for Python import
set_target_properties(gegelapy PROPERTIES
    PREFIX ""
    OUTPUT_NAME "gegelapy"
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/gegelapy
    BUILD_WITH_INSTALL_RPATH TRUE
    INSTALL_RPATH "@loader_path"
)

add_custom_command(TARGET gegelapy POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:GEGELATI>
    $<TARGET_FILE_DIR:gegelapy>
)
