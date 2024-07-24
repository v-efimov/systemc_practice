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
    bfm.valid_if(tb.valid_p);
    bfm.data_if(tb.data_p);
    tb.ready_if(bfm.ready_p);
    bfm.input_if(tlm.input_p);

    sc_start(51, SC_SEC);
    return 0;
}
