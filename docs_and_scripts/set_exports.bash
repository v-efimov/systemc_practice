#!/usr/bin/env bash

# RUN THIS AS source ./set_exports.bash #
#These are required to build Verilator and SystemC and run Verilator together with SystemC
#If necessary change c++14/gnu++14 to c++17/gnu++17
export CXX="g++"
export CXXFLAGS="-std=c++14"
export CFG_CXXFLAGS_STD_NEWEST="-std=gnu++14"
export LD_LIBRARY_PATH=`pwd`"/systemc-2.3.3/lib-linux64"
export SYSTEMC_INCLUDE=`pwd`"/systemc-2.3.3/include"
export SYSTEMC_LIBDIR=`pwd`"/systemc-2.3.3/lib-linux64"
export VERILATOR_ROOT=`pwd`"/verilator"
#These are required to run SystemC examples without Verilator
export SYSTEMC_HOME=`pwd`"/systemc-2.3.3"
export TARGET_ARCH="linux64"

