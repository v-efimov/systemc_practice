#include "systemc.h"
#include "bfm.h"

void bfm_module::bfm_axim_method() {
    if (rst_port->read() == true) {
       AXIM_valid_port->write(true);
    }
    else
    {
      //we are comparing pre-clock value here.
      //pipeline drives if ~down_valid or down_ready
      if (AXIM_valid_port->read() == false || AXIM_ready_port->read() == true) {
        if (PIPES_port->nb_read(ReadValueFromPIPES) == true) {
          AXIM_valid_port->write(true);
          AXIM_data_port->write(ReadValueFromPIPES);
        }
        else
        {
          AXIM_valid_port->write(false);
          //we only modify value of valid and don't modify value of data signal
        }
      }
    }
}
