######################################################################
#
# DESCRIPTION: Verilator CMake Example: Small CMakeLists.txt with SystemC
#
# This is an example cmake script to build a verilog to SystemC project
# using CMake and Verilator.
#
# This file ONLY is placed under the Creative Commons Public Domain, for
# any use, without warranty, 2020 by Wilson Snyder.
# SPDX-License-Identifier: CC0-1.0
#
######################################################################

# This example builds the tracing_sc example using CMake
# To use it, run the following:

# cd /path/to/verilator/examples/cmake_sc
# rm -rf build && mkdir build && cd build
# cmake ..
# cmake --build .

cmake_minimum_required(VERSION 3.12)
cmake_policy(SET CMP0074 NEW)
project(cmake_hello_sc CXX)

find_package(verilator HINTS $ENV{VERILATOR_ROOT} ${VERILATOR_ROOT})
if(NOT verilator_FOUND)
    message(
        FATAL_ERROR
        "Verilator was not found. Either install it, or set the VERILATOR_ROOT environment variable"
    )
endif()

file (GLOB SOURCES
    ./*.cpp
#     /home/va/Verilator_503_Systemc_301_cpp17/systemc/scv/*.h
)

add_executable(example ${SOURCES})
target_compile_features(example PUBLIC cxx_std_17)

set_property(TARGET example PROPERTY CXX_STANDARD ${SystemC_CXX_STANDARD})

# Add the Verilated circuit to the target
verilate(example SYSTEMC
  INCLUDE_DIRS ./
  VERILATOR_ARGS -f ./input.vc -x-assign fast --trace --assert --coverage
  SOURCES ./top.v
)

verilator_link_systemc(example)
add_library(scv_lib SHARED IMPORTED)
set_target_properties(scv_lib PROPERTIES IMPORTED_LOCATION /home/va/Verilator_503_Systemc_301_cpp17/systemc/lib-linux64/libscv.so)
target_link_libraries(example PUBLIC scv_lib)
