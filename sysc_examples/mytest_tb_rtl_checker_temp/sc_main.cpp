// SystemC global header
#include <systemc.h>
#include "tb.h"
#include "bfm.h"
#include "tlm.h"

using namespace sc_core;


int sc_main(int, char* []) {
    sc_clock clk ("clk", 10, SC_SEC, 0.2, 10, SC_SEC, false);
    TB tb("tb");
    BFM bfm("bfm");
    TLM tlm("tlm");
    tb.clk(clk);
    bfm.clk(clk);
    bfm.AXIS_valid_if(tb.AXIM_valid_p);
    bfm.AXIS_data_if(tb.AXIM_data_p);
    tb.AXIM_ready_if(bfm.AXIS_ready_p);
    bfm.PIPEM_if(tlm.PIPES_p);
    bfm.PIPES_if(tlm.PIPEM_p);

    sc_start(151, SC_SEC);
    return 0;
}
