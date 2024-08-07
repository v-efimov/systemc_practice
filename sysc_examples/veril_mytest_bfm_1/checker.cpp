#include "systemc.h"
#include "checker.h"

void CHECKER::checker_process() {
  int ready_probability = 5;
  while (true) {
    //we are comparing pre-clock value here.
    //check if handshake happened then push to TLM FIFO
    if (AXIS_valid_port->read() == true && AXIS_ready_ch.read() == true) {
        std::cout << sc_time_stamp() << ": checker reads from AXIS signal channel: valid =" << AXIS_valid_port->read() << ", data =" << AXIS_data_port->read() << std::endl;
    }
    //Ready can toggle on its own
    if (std::rand() % 100 <= ready_probability) {
        AXIS_ready_ch.write(true);
    }
    else
    {
        AXIS_ready_ch.write(false);
    }
    wait();
  }
}