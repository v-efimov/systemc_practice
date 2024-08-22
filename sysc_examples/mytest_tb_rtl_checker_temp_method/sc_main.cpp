// SystemC global header
#include <systemc.h>
#include "tb.h"
#include "bfm.h"
#include "tlm.h"
#include "checker.h"

using namespace sc_core;

//#define TRACE_SIG(F, X) sc_core::sc_trace(F, X, X.name())

int sc_main(int, char* []) {
    sc_clock clk ("clk", 10, SC_SEC, 0.2, 10, SC_SEC, false);
    TB tb("tb");
    BFM bfm("bfm");
    TLM tlm("tlm");
    CHECKER checker("checker");
    tb.clk(clk);
    bfm.clk(clk);
    checker.clk(clk);
    bfm.AXIS_valid_if(tb.AXIM_valid_p);
    bfm.AXIS_data_if(tb.AXIM_data_p);
    tb.AXIM_ready_if(bfm.AXIS_ready_p);
    bfm.PIPEM_if(tlm.PIPES_p);
    bfm.PIPES_if(tlm.PIPEM_p);
    checker.AXIS_valid_if(bfm.AXIM_valid_p);
    checker.AXIS_data_if(bfm.AXIM_data_p);
    bfm.AXIM_ready_if(checker.AXIS_ready_p);

    //sc_trace_file* file = sc_create_vcd_trace_file("trace");
    sc_start(10151, SC_SEC);
    //sc_close_vcd_trace_file(file);

    return 0;
}
