cmake_minimum_required(VERSION 3.14)
project(GeneratedTPG LANGUAGES C CXX)

find_package(pybind11 REQUIRED)

pybind11_add_module(gen_tpg src/generated_tpg.c src/generated_tpg_program.c src/wrapper.cpp)
set_target_properties(gen_tpg PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
