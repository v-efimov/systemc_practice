// SystemC global header
#include <systemc.h>
#include "generator.h"
#include "arbiter.h"
#include "monitor.h"

using namespace sc_core;


int sc_main(int, char* []) {
    GENERATOR module1("module1");
    ARBITER module2("module2");
    MONITOR module3("module3");
    module2.inp1(module1.p1);
    module2.inp2(module1.p2);
    module2.outp1(module3.inp1);

    sc_start(10, SC_SEC);
    return 0;
}
