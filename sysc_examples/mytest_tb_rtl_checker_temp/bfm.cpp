#include "systemc.h"
#include "bfm.h"

void BFM::bfm_process() {
  int ReadValueFromPIPES;
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
        std::cout << sc_time_stamp() << ": HANDHSAKE HAPPENED DOSPOSE PREV VALUE =" << "-----" << std::endl;
        PIPES_if->read();
    }
    //check if there is value in TLM FIFO, If so then set valid
    if (PIPES_if->nb_read(ReadValueFromPIPES) == true) {
        std::cout << sc_time_stamp() << ": BFM noticed value =" << ReadValueFromPIPES << "-----" << std::endl;
        AXIM_valid_ch.write(true);
        AXIM_data_ch.write(ReadValueFromPIPES);
    }
    else
    {
        AXIM_valid_ch.write(false);
        //we only modify value of valid and don't modify value of data signal
    }

    ///std::cout << sc_time_stamp() << ": reads from AXIS signal channel, valid =" << AXIS_valid_if->read() << ", data =" << AXIS_data_if->read() << std::endl;
    wait();
  }
}