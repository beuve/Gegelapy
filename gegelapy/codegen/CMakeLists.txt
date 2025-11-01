cmake_minimum_required(VERSION 3.14)
project(GeneratedTPG LANGUAGES C CXX)

find_package(pybind11 REQUIRED)

add_library(gen_tpg MODULE src/generated_tpg.c src/generated_tpg_program.c src/wrapper.cpp)
target_link_libraries(gen_tpg PRIVATE pybind11::module)
set_target_properties(gen_tpg PROPERTIES 
  PREFIX "" 
  OUTPUT_NAME "gen_tpg"
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
