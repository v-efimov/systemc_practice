// SystemC global header
#include <systemc.h>
#include "tb.h"
#include "bfm.h"

using namespace sc_core;


int sc_main(int, char* []) {
    sc_clock clk ("clk", 10, SC_SEC, 0.2, 10, SC_SEC, false);
    TB tb("tb");
    BFM bfm("bfm");
    tb.clk(clk);
    bfm.clk(clk);
    bfm.data_if(tb.data_p);

    sc_start(51, SC_SEC);
    return 0;
}
