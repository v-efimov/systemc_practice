// SystemC global header
#include <systemc.h>
#include "module1.h"
#include "module2.h"

//#include "mytype.h"

using namespace sc_core;

int sc_main(int, char* []) {
    MODULE1 module1("module1");
    MODULE2 module2("module2");
    module2.p(module1.p);

    sc_start(10, SC_SEC);
    return 0;
}
