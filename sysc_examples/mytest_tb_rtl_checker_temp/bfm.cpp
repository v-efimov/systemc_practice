#include "systemc.h"
#include "bfm.h"

void BFM::bfm_process() {
  while (true) {
    //we are comparing pre-clock value here.
    //check if handshake happened then push to TLM FIFO
    if (AXIS_valid_if->read() == true && AXIS_ready_ch.read() == true) {
        PIPEM_if->write(AXIS_data_if->read());
    }
    //check if there is space in TLM FIFO. If so then set ready
    if (PIPEM_if->num_free() == 0) {
        AXIS_ready_ch.write(false);
    }
    else
    {
       AXIS_ready_ch.write(true);
    }

    //we are comparing pre-clock value here.
    //check if handshake happened then pop from TLM FIFO
    if (AXIM_valid_ch.read() == true && AXIM_ready_if->read() == true) {
        PIPES_if->read();
    }
    //check if there is value in TLM FIFO, If so then set valid

    //DO NON-BLOCKING READ HERE *****
    if (PIPES_if->num_available() == 0) {
        AXIM_valid_ch.write(false);
        AXIM_data_ch.write(0);
    }
    else
    {
       AXIM_valid_ch.write(true);
    }
    //**********

    std::cout << sc_time_stamp() << ": reads from signal channel, valid =" << AXIS_valid_if->read() << ", data =" << AXIS_data_if->read() << std::endl;
    wait();
  }
}