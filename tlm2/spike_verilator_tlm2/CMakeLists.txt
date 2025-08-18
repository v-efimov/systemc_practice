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

set(SYSTEMC_HOME $ENV{SYSTEMC_HOME})
if(NOT SYSTEMC_HOME)
    message(FATAL_ERROR "Environment variable SYSTEMC_HOME is not set")
endif()

# Include and lib paths
include_directories(${SYSTEMC_HOME}/include)
link_directories(${SYSTEMC_HOME}/lib-linux64)

#Link SCV
add_library(scv_lib SHARED IMPORTED)
set_target_properties(scv_lib PROPERTIES IMPORTED_LOCATION ${SYSTEMC_HOME}/lib-linux64/libscv.so)
target_link_libraries(example PUBLIC scv_lib)
