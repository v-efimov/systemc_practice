#include <systemc>

#include "Vtop.h"
#include "verilated_vcd_sc.h"

SC_MODULE(Testbench) {
    sc_core::sc_out<bool> rst;

    SC_CTOR(Testbench) {
        SC_THREAD(tb);
    }

    void tb() {
        rst = true;
        wait(10, sc_core::SC_NS);
        rst = false;
    }
};

int sc_main(int argc, char* argv[]) {
    Verilated::commandArgs(argc, argv);

    sc_core::sc_signal<bool> rst;
    sc_core::sc_clock clk("clk", 10, sc_core::SC_NS, 0.5, 0, sc_core::SC_NS, true);

    Vtop* top = new Vtop{"top"};
    top->clk        (clk);
    top->rst        (rst);

    Testbench tb("tb");
    tb.rst (rst);

    Verilated::traceEverOn(true);
    VerilatedVcdSc* tfp = new VerilatedVcdSc;
    sc_core::sc_start(sc_core::SC_ZERO_TIME);
    top->trace(tfp, 99);
    tfp->open("logs/wave.vcd");

    sc_core::sc_start(200, sc_core::SC_NS);

    if (tfp) {
        tfp->close();
        tfp = nullptr;
    }
    delete top;

    Verilated::mkdir("logs");
    VerilatedCov::write("logs/coverage.dat");

    return 0;
}
