cmake_minimum_required(VERSION 3.10)
project(MySystemCExample)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SYSTEMC_HOME $ENV{SYSTEMC_HOME})
if(NOT SYSTEMC_HOME)
    message(FATAL_ERROR "Environment variable SYSTEMC_HOME is not set")
endif()


# Create executable
add_executable(systemc_example)

target_sources(systemc_example PRIVATE
    ./sc_main.cpp
)

#Include paths
target_include_directories(systemc_example PRIVATE ${SYSTEMC_HOME}/include)

#Specify SystemC library
add_library(systemc_lib SHARED IMPORTED)
set_target_properties(systemc_lib PROPERTIES IMPORTED_LOCATION ${SYSTEMC_HOME}/lib-linux64/libsystemc.so)
#Specify SCV library
add_library(scv_lib SHARED IMPORTED)
set_target_properties(scv_lib PROPERTIES IMPORTED_LOCATION ${SYSTEMC_HOME}/lib-linux64/libscv.so)

#Link libraries
target_link_libraries(systemc_example PRIVATE systemc_lib scv_lib)