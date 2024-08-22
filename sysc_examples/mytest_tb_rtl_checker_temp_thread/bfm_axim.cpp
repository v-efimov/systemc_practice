#include "systemc.h"
#include "bfm.h"

void BFM::bfm_process_axis() {
  int REG;
  enum BufState { BYPASS, SKID } state;
  state = BYPASS;
  AXIS_ready_ch.write(true);
  //skid-buffer
  //We shall not have blocking read or writes from FIFOs in BFM
  while (true) {
    //we are comparing pre-clock value here.
    //if handshake happened while FIFO buffer is not in SKID
    if (state == BYPASS && AXIS_valid_if->read() == true && AXIS_ready_ch.read() == true) {
        std::cout << sc_time_stamp() << "HANDSHAKE WITH BYPASS" << std::endl;
        if (PIPEM_if->nb_write(AXIS_data_if->read()) == false) {
            REG = AXIS_data_if->read();
            state = SKID;
            AXIS_ready_ch.write(false);
        }
    }
    if (state == SKID) {
        std::cout << sc_time_stamp() << "SKIDDING" << std::endl;
        if (PIPEM_if->nb_write(REG) == true) {
           state = BYPASS;
           AXIS_ready_ch.write(true);
        }
    }
    wait();
  }
}
