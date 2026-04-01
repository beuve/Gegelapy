cmake_minimum_required(VERSION 3.14)
project(gegelapy LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(SKIP_DOXYGEN_BUILD ON CACHE BOOL "" FORCE)
set(OUTPUT_DIR "${CMAKE_SOURCE_DIR}/gegelapy")

find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
add_subdirectory(extern/gegelati)
add_subdirectory(extern/pybind11)

if(WIN32)
  set(ENV_FILE "src/environments/win32GymEnvironment.cpp")
else()
  set(ENV_FILE "src/environments/posixGymEnvironment.cpp")
endif()

pybind11_add_module(gegelapy
    src/gegelapy.cpp
    ${ENV_FILE}
    src/trainer.cpp
    src/instructions.cpp
    src/outputConversions.cpp
    src/algorithms/algorithm.cpp
)

target_link_libraries(gegelapy PRIVATE GEGELATI)
set_target_properties(gegelapy GEGELATI PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_DIR}"
    RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}"
)
target_include_directories(gegelapy PUBLIC include)

if(APPLE)
    set_target_properties(gegelapy PROPERTIES 
        INSTALL_RPATH "@loader_path"
        BUILD_WITH_INSTALL_RPATH TRUE
    )
    set_target_properties(GEGELATI PROPERTIES 
        INSTALL_NAME_DIR "@rpath"
    )
endif()

install(TARGETS gegelapy GEGELATI DESTINATION gegelapy)
