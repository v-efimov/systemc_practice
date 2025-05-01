// -*- SystemC -*-
// DESCRIPTION: Verilator Example: Top level main for invoking SystemC model
//
// This file ONLY is placed under the Creative Commons Public Domain, for
// any use, without warranty, 2017 by Wilson Snyder.
// SPDX-License-Identifier: CC0-1.0
//======================================================================

#include <iostream>

// For std::unique_ptr
#include <memory>

// SystemC global header
#include <systemc.h>

// Include common routines
#include <verilated.h>
#if VM_TRACE
#include <verilated_vcd_sc.h>
#endif

#include <sys/stat.h>  // mkdir

// Include model header, generated from Verilating "top.v"
#include "Vtop.h"

#define VERBOSE 0

class stage_tracer : sc_module, sc_stage_callback_if
{
public:
  stage_tracer( sc_module_name nm
                  = sc_core::sc_gen_unique_name("stage_tracer") )
    : cb_count(0), upd_count(0)
  {
    //sc_register_stage_callback( *this, SC_POST_BEFORE_END_OF_ELABORATION );
    //sc_register_stage_callback( *this, SC_POST_END_OF_ELABORATION );
    //sc_register_stage_callback( *this, SC_POST_START_OF_SIMULATION );
    sc_register_stage_callback( *this, SC_POST_UPDATE );
    sc_register_stage_callback( *this, SC_PRE_TIMESTEP );
    //sc_register_stage_callback( *this, SC_PRE_PAUSE );
    //sc_register_stage_callback( *this, SC_PRE_SUSPEND );  // not tested here
    //sc_register_stage_callback( *this, SC_POST_SUSPEND ); // not tested here
    //sc_register_stage_callback( *this, SC_PRE_STOP );
    //sc_register_stage_callback( *this, SC_POST_END_OF_SIMULATION );
  }

  virtual void stage_callback(const sc_stage& stage)
  {

    switch( stage )
    {
      case SC_POST_UPDATE:   // for SC_POST_UPDATE..
        std::cout << "CASE_POST_UPDATE" << upd_count <<std::endl;
        upd_count++;
      case SC_PRE_TIMESTEP:  //  ..and SC_PRE_TIMESTEP
        cb_count++;
        std::cout << "CASE_PRE_TIMESTEP" << cb_count <<std::endl;
        break;
      default:
        // do nothing
        break;
    }
 }

  ~stage_tracer()
  {
    //sc_unregister_stage_callback( *this, SC_POST_BEFORE_END_OF_ELABORATION );
    //sc_unregister_stage_callback( *this, SC_POST_END_OF_ELABORATION );
    //sc_unregister_stage_callback( *this, SC_POST_START_OF_SIMULATION );
    sc_unregister_stage_callback( *this, SC_POST_UPDATE );
    sc_unregister_stage_callback( *this, SC_PRE_TIMESTEP );
    //sc_unregister_stage_callback( *this, SC_PRE_PAUSE );
    //sc_unregister_stage_callback( *this, SC_PRE_SUSPEND );
    //sc_unregister_stage_callback( *this, SC_POST_SUSPEND );
    //sc_unregister_stage_callback( *this, SC_PRE_STOP );
    //sc_unregister_stage_callback( *this, SC_POST_END_OF_SIMULATION );
  }

private:
  sc_dt::uint64 cb_count, upd_count;
};



int sc_main(int argc, char* argv[]) {

    stage_tracer tracer("tracer");

    // This is a more complicated example, please also see the simpler examples/make_hello_c.

    // Prevent unused variable warnings
    if (false && argc && argv) {}

    // Create logs/ directory in case we have traces to put under it
    Verilated::mkdir("logs");

    // Set debug level, 0 is off, 9 is highest presently used
    // May be overridden by commandArgs argument parsing
    Verilated::debug(0);

    // Randomization reset policy
    // May be overridden by commandArgs argument parsing
    Verilated::randReset(2);

#if VM_TRACE
    // Before any evaluation, need to know to calculate those signals only used for tracing
    Verilated::traceEverOn(true);
#endif

    // Pass arguments so Verilated code can see them, e.g. $value$plusargs
    // This needs to be called before you create any model
    Verilated::commandArgs(argc, argv);

    // General logfile
    ios::sync_with_stdio();

    // Define clocks
    sc_clock clk_ch{"clk", 10, SC_NS, 0.5, 3, SC_NS, true};

    // Define interconnect
    sc_signal<bool> reset;
    sc_signal<bool> gen_down_valid;
    sc_signal<std::uint32_t> gen_down_data;
    sc_signal<bool> gen_down_ready;
    sc_signal<bool> chk_up_valid;
    sc_signal<std::uint32_t> chk_up_data;
    sc_signal<bool> chk_up_ready;

    // Construct the Verilated model, from inside Vtop.h
    // Using unique_ptr is similar to "Vtop* top = new Vtop" then deleting at end
    const std::unique_ptr<Vtop> top{new Vtop{"top"}};

    // Attach Vtop's signals to this upper model
    top->clk(clk_ch);
    top->rst(reset);
    top->gen_down_valid(gen_down_valid);
    top->gen_down_data(gen_down_data);
    top->gen_down_ready(gen_down_ready);
    top->chk_up_valid(chk_up_valid);
    top->chk_up_data(chk_up_data);
    top->chk_up_ready(chk_up_ready);

    // You must do one evaluation before enabling waves, in order to allow
    // SystemC to interconnect everything for testing.
    sc_start(1, SC_NS);

#if VM_TRACE
    // If verilator was invoked with --trace argument,
    // and if at run time passed the +trace argument, turn on tracing
    VerilatedVcdSc* tfp = nullptr;
    const char* flag = Verilated::commandArgsPlusMatch("trace");
    if (flag && 0 == strcmp(flag, "+trace")) {
        cout << "Enabling waves into logs/vlt_dump.vcd...\n";
        tfp = new VerilatedVcdSc;
        top->trace(tfp, 99);  // Trace 99 levels of hierarchy
        Verilated::mkdir("logs");
        tfp->open("logs/vlt_dump.vcd");
    }
#endif

    //// Simulate until $finish
    //while (!Verilated::gotFinish()) {

    // Simulate until simulation_done reached
    uint simulation_done = 0;
    while (simulation_done < 10000) {
#if VM_TRACE
        // Flush the wave files each cycle so we can immediately see the output
        // Don't do this in "real" programs, do it in an abort() handler instead
        if (tfp) tfp->flush();
#endif

        // Apply inputs
        if (sc_time_stamp() > sc_time(1, SC_NS) && sc_time_stamp() < sc_time(10, SC_NS)) {
            reset = 1;  // Assert reset
        } else {
            reset = 0;  // Deassert reset
        }

        // Simulate 1ns
        sc_start(1, SC_NS);
        simulation_done +=1;
    }

    // Final model cleanup
    top->final();

    // Close trace if opened
#if VM_TRACE
    if (tfp) {
        tfp->close();
        tfp = nullptr;
    }
#endif

    // Coverage analysis (calling write only after the test is known to pass)
#if VM_COVERAGE
    Verilated::mkdir("logs");
    VerilatedCov::write("logs/coverage.dat");
#endif

    // Return good completion status
    return 0;
}
