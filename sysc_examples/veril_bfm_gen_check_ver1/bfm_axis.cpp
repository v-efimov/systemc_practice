#include "systemc.h"
#include "bfm.h"

void BFM::bfm_process_axis() {
    if (rst_port->read() == true) {
       state = BYPASS;
       AXIS_ready_port->write(true);
    } 
    else
    {
      //we are comparing pre-clock value here.
      //if handshake happened while FIFO buffer is not in SKID
      if (state == BYPASS && AXIS_valid_port->read() == true && AXIS_ready_port->read() == true) {
          //std::cout << sc_time_stamp() << "HANDSHAKE WITH BYPASS" << std::endl;
          if (PIPEM_port->nb_write(AXIS_data_port->read()) == false) {
              REG = AXIS_data_port->read();
              state = SKID;
              AXIS_ready_port->write(false);
          }
      }
      if (state == SKID) {
          //std::cout << sc_time_stamp() << "SKIDDING" << std::endl;
          if (PIPEM_port->nb_write(REG) == true) {
             state = BYPASS;
             AXIS_ready_port->write(true);
          }
      }
    }
}
