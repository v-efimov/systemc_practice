#include "systemc.h"
#include "simcontrol.h"

void simcontrol_module::start_of_simulation(void) {
    rstn_port->write(false);
    stopsim_port->write(false);
    std::cout << sc_time_stamp() << "Initial (RESET_ON)" << std::endl;
}

void simcontrol_module::simcontrol_thread() {
    wait(15, SC_NS);
    rstn_port->write(true);
    std::cout << sc_time_stamp() << "Start Simulation (RESET_OFF)" << std::endl;
    wait(15000, SC_NS);           //Run simulation
    std::cout << sc_time_stamp() << "Please FINISH Simulation (STOPSIM_ON)" << std::endl;
    stopsim_port->write(true);   //finish simulation
}
