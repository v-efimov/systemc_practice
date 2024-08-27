#include "systemc.h"
#include "bfm.h"

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
          if (PIPEM_port->nb_write(AXIS_data_port->read()) == false) {
              REG = AXIS_data_port->read();
              state.write(BufState::SKID);
              AXIS_ready_port->write(false);
          }
      }
      if (state.read() == BufState::SKID) {
          if (PIPEM_port->nb_write(REG) == true) {
             state.write(BufState::BYPASS);
             AXIS_ready_port->write(true);
          }
      }
    }
}
