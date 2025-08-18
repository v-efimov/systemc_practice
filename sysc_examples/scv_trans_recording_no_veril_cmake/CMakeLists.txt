cmake_minimum_required(VERSION 3.10)
project(MySystemCExample)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SYSTEMC_HOME $ENV{SYSTEMC_HOME})
if(NOT SYSTEMC_HOME)
    message(FATAL_ERROR "Environment variable SYSTEMC_HOME is not set")
endif()

# Include and lib paths
include_directories(${SYSTEMC_HOME}/include)
link_directories(${SYSTEMC_HOME}/lib-linux64)

# Source files
file(GLOB SOURCES
    ./*.cpp
)

# Create executable
add_executable(systemc_example ${SOURCES})

# Link SystemC
target_link_libraries(systemc_example PUBLIC systemc)

#Link SCV
add_library(scv_lib SHARED IMPORTED)
set_target_properties(scv_lib PROPERTIES IMPORTED_LOCATION ${SYSTEMC_HOME}/lib-linux64/libscv.so)
target_link_libraries(systemc_example PUBLIC scv_lib)
