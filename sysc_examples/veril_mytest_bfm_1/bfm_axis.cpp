#include "systemc.h"
#include "bfm.h"

void BFM::bfm_process_axim() {
  std::uint32_t ReadValueFromPIPES;
  //output register
  //We shall not have blocking read or writes from FIFOs in BFM
  while (true) {
    //we are comparing pre-clock value here.
    //pipeline drives if ~down_valid or down_ready
    if (AXIM_valid_ch.read() == false || AXIM_ready_port->read() == true) {
      if (PIPES_port->nb_read(ReadValueFromPIPES) == true) {
        AXIM_valid_ch.write(true);
        AXIM_data_ch.write(ReadValueFromPIPES);
      }
      else
      {
        AXIM_valid_ch.write(false);
        //we only modify value of valid and don't modify value of data signal
      }
    }
    wait();
  }
}
