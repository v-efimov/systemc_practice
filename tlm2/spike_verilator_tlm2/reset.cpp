#include "systemc.h"
#include "reset.h"

void reset_module::start_of_simulation() {
    rstn_port->write(false);
    std::cout << sc_time_stamp() << "Initial (RESET_ON)" << std::endl;
}

void reset_module::reset_thread() {
    wait(15, SC_NS);
    rstn_port->write(true);
}
