#include "systemc.h"
#include "bfm.h"

void BFM::bfm_process() {
  while (true) {
    if (valid_if->read() == true && ready_ch.read() == true) {
        //we are comparing pre-clock value here.
        //check if handshake happened then push to TLM FIFO
        input_if->write(data_if->read());
    }
    //check if there is space in TLM FIFO
    if (input_if->num_free() == 0) {
        //we are comparing pre-clock value here.
        //check if handshake happened then push to TLM FIFO
        ready_ch.write(false);
    }
    else
    {
       ready_ch.write(true);
    }

    ///??????if handshake on output fifo happened then?????
    ///pop from TLM FIFO
    ///????????????????????????????????????????????????????

 
    std::cout << sc_time_stamp() << ": reads from signal channel, valid =" << valid_if->read() << ", data =" << data_if->read() << std::endl;
    wait();
  }
}