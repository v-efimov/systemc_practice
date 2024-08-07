#include "systemc.h"
#include "bfm.h"

void BFM::bfm_process() {
  while (true) {
    if (AXIS_valid_if->read() == true && AXIS_ready_ch.read() == true) {
        //we are comparing pre-clock value here.
        //check if handshake happened then push to TLM FIFO
        PIPEM_if->write(AXIS_data_if->read());
    }
    //check if there is space in TLM FIFO
    if (PIPEM_if->num_free() == 0) {
        //we are comparing pre-clock value here.
        //check if handshake happened then push to TLM FIFO
        AXIS_ready_ch.write(false);
    }
    else
    {
       AXIS_ready_ch.write(true);
    }

    ///??????if handshake on output fifo happened then?????
    ///pop from TLM FIFO
    ///????????????????????????????????????????????????????


    std::cout << sc_time_stamp() << ": reads from signal channel, valid =" << AXIS_valid_if->read() << ", data =" << AXIS_data_if->read() << std::endl;
    wait();
  }
}