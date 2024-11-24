#include "systemc.h"
#include "bfm.h"

void bfm_module::bfm_axis_method() {
    if (rst_port->read() == true) {
       state.write(BufState::BYPASS);
       AXIS_ready_port->write(true);
    }
    else
    {
      //we are comparing pre-clock value here.
      //Write data into expansion REGs if handshake happens but no free place in PIPEM_FIFO
      if (state.read() == BufState::BYPASS) {
          if (AXIS_valid_port->read() == true && AXIS_ready_port->read() == true) {
            if (PIPEM_port->nb_write(AXIS_data_port->read()) == false) {
                REG.write(AXIS_data_port->read());
                state.write(BufState::SKID);
                AXIS_ready_port->write(false);
            }
          }
      }
      else if (state.read() == BufState::SKID) {
          if (PIPEM_port->nb_write(REG) == true) {
             state.write(BufState::BYPASS);
             AXIS_ready_port->write(true);
          }
      }
      else {}
    }
}
