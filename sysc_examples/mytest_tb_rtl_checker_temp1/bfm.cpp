#include "systemc.h"
#include "bfm.h"

void BFM::bfm_process() {
  int ReadValueFromPIPES;
  AXIM_valid_ch.write(false);
  while (true) {
    //we are comparing pre-clock value here.
    //check if handshake happened then push to TLM FIFO
    if (AXIS_valid_if->read() == true && AXIS_ready_ch.read() == true) {
        PIPEM_if->write(AXIS_data_if->read());
        std::cout << sc_time_stamp() << ": 1-BFM AXIS HANDHSAKE HAPPENED" << "-----" << std::endl;
    }
    else
    {
        std::cout << sc_time_stamp() << ": 1-BFM AXIS HANDHSAKE NOT HAPPENED" << "-----" << std::endl;
    }
    //check if there is space in TLM FIFO. If so then set ready
    if (PIPEM_if->num_free() == 0) {
        AXIS_ready_ch.write(false);
        std::cout << sc_time_stamp() << ": 2-BFM AXIS NO SPACE IN FIFO" << "-----" << std::endl;
    }
    else
    {
       AXIS_ready_ch.write(true);
       std::cout << sc_time_stamp() << ": 2-BFM AXIS SPACE IN FIFO" << "-----" << std::endl;
    }

    //we are comparing pre-clock value here.
    //check if handshake happened then pop from TLM FIFO
    //if (AXIM_valid_ch.read() == false || AXIM_ready_if->read() == true) {
    if (true == true) {
        if (PIPES_if->nb_read(ReadValueFromPIPES)) {
        std::cout << sc_time_stamp() << "VALUE IN FFFFFIIIIFFFOOOO: " << ReadValueFromPIPES << " -----" << std::endl;
        AXIM_valid_ch.write(true);
        AXIM_data_ch.write(ReadValueFromPIPES);
        PIPES_if->read();
        }
        else
        {
        AXIM_valid_ch.write(false);
        }
    }
    else
    {
        std::cout << sc_time_stamp() << ": 3-BFM AXIM HANDHSAKE NOT HAPPENED" << "-----" << std::endl;
    }
    wait();
  }
}