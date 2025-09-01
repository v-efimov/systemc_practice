//============== Modifyed Verilator SystemC top module =================
//======================Original header=================================
// -*- SystemC -*-
// DESCRIPTION: Verilator Example: Top level main for invoking SystemC model
//
// This file ONLY is placed under the Creative Commons Public Domain, for
// any use, without warranty, 2017 by Wilson Snyder.
// SPDX-License-Identifier: CC0-1.0
//======================================================================

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
#include "reset.h"
#include "pins2queue.h"
#include "queue2pins.h"
#include "uncore.h"
#include "spike.h"

int sc_main(int argc, char* argv[]) {

  try {
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
    sc_signal<bool> resetn;
    sc_signal<bool> stopsim;

    sc_signal<bool> up_valid;
    sc_signal<std::uint32_t> up_data;
    sc_signal<bool> up_ready;
    sc_signal<bool> down_valid;
    sc_signal<std::uint32_t> down_data;
    sc_signal<bool> down_ready;


    // Construct the Verilated model, from inside Vtop.h
    // Using unique_ptr is similar to "Vtop* veritop = new Vtop" then deleting at end
    const std::unique_ptr<Vtop> veritop{new Vtop{"veritop"}};
    const std::unique_ptr<spike_module> spike{new spike_module{"spike"}};
    const std::unique_ptr<uncore_module> uncore{new uncore_module{"uncore"}};

    const std::unique_ptr<reset_module> reset{new reset_module{"reset"}};
    const std::unique_ptr<queue2pins_module<std::uint32_t>> queue2pins{new queue2pins_module<std::uint32_t>{"queue2pins"}};
    const std::unique_ptr<pins2queue_module<std::uint32_t>> pins2queue{new pins2queue_module<std::uint32_t>{"pins2queue"}};


    spike->spike_uncore_socket.bind(uncore->spike_uncore_socket);

    queue2pins->clk_port(clk_ch);
    queue2pins->rstn_port(resetn);
    queue2pins->down_valid(up_valid);
    queue2pins->down_data(up_data);
    queue2pins->down_ready(up_ready);
    queue2pins->up_PIPE(uncore->req_PIPE);


    pins2queue->clk_port(clk_ch);
    pins2queue->rstn_port(resetn);
    pins2queue->up_valid(down_valid);
    pins2queue->up_data(down_data);
    pins2queue->up_ready(down_ready);
    pins2queue->down_PIPE(uncore->resp_PIPE);


    reset->rstn_port(resetn);
    uncore->stopsim_port(stopsim);


    // Attach Vtop's signals to this upper model
    veritop->clk(clk_ch);
    veritop->rst_n(resetn);
    veritop->up_valid(up_valid);
    veritop->up_data(up_data);
    veritop->up_ready(up_ready);
    veritop->down_valid(down_valid);
    veritop->down_data(down_data);
    veritop->down_ready(down_ready);


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
        veritop->trace(tfp, 99);  // Trace 99 levels of hierarchy
        Verilated::mkdir("logs");
        tfp->open("logs/vlt_dump.vcd");
    }
#endif

    // Simulate until simulation_done reached
    while (stopsim.read() == false) {
#if VM_TRACE
        // Flush the wave files each cycle so we can immediately see the output
        // Don't do this in "real" programs, do it in an abort() handler instead
        if (tfp) tfp->flush();
#endif
        // Simulate 1ns
        sc_start(50, SC_NS);
    }

    // Final model cleanup
    veritop->final();

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

  } catch (const sc_report& e) {
    std::cerr << "Caught SystemC exception: " << e.what() << std::endl;
    return 1;
  }
  // Return good completion status
  return 0;
}
