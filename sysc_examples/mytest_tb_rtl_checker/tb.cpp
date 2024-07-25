#include "systemc.h"
#include "tb.h"

void TB::tb_process() {
  int valid_probability = 75;
  int data = 1;
  while (true) {
    if (AXIM_valid_ch.read() == true && AXIM_ready_if->read() == false) {
    //we are comparing pre-clock value here.
    //if valid that is set just after clock on previous cycle has not been handshaked in the current cycle then keep valid up
    }
    else
    {
    //otherwise either handshake happened or there was no valid before so we
    //generate new random valid here
    //if valid equal to 1 then assign new data
        if (std::rand() % 100 <= valid_probability) {
            AXIM_valid_ch.write(true);
            AXIM_data_ch.write(data++);
        }
        else
        {
            AXIM_valid_ch.write(false);
        }
    }
    std::cout << sc_time_stamp() << ", prev_valid =" << AXIM_valid_ch.read() << ", prev_data =" << AXIM_data_ch.read() << ", prev_ready =" << AXIM_ready_if->read() <<std::endl;
    wait();
  }
}
