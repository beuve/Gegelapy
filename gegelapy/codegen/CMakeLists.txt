cmake_minimum_required(VERSION 3.14)
project(GeneratedTPG LANGUAGES C CXX)

find_package(pybind11 REQUIRED)
FILE(GLOB ALGO_SRC src/*.c)
pybind11_add_module(gen_tpg ${ALGO_SRC} src/wrapper.cpp)
set_target_properties(gen_tpg PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
