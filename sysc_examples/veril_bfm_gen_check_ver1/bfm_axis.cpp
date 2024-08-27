#include "systemc.h"
#include "bfm.h"

//BYPASS = false;
//SKID = true;

void BFM::bfm_process_axis() {
    if (rst_port->read() == true) {
       state.write(BufState::BYPASS);
       AXIS_ready_port->write(true);
    }
    else
    {
      //we are comparing pre-clock value here.
      //if handshake happened while FIFO buffer is not in SKID
      if (state.read() == BufState::BYPASS && AXIS_valid_port->read() == true && AXIS_ready_port->read() == true) {
          std::cout << sc_time_stamp() << "HANDSHAKE WITH BYPASS" << AXIS_data_port->read() << std::endl;
          if (PIPEM_port->nb_write(AXIS_data_port->read()) == false) {
              std::cout << sc_time_stamp() << "NB_WRITE_FALSE_GO_TO_SKIDDING" << std::endl;
              REG = AXIS_data_port->read();
              state.write(BufState::SKID);
              AXIS_ready_port->write(false);
          }
      }
      if (state.read() == BufState::SKID) {
          std::cout << sc_time_stamp() << "SKIDDING" << std::endl;
          if (PIPEM_port->nb_write(REG) == true) {
             std::cout << sc_time_stamp() << "SKIDDING_TRUE_WRITE" << AXIS_data_port->read() << std::endl;
             state.write(BufState::BYPASS);
             AXIS_ready_port->write(true);
          }
      }
    }
}
