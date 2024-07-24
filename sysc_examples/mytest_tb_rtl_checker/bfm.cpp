#include "systemc.h"
#include "bfm.h"

void BFM::bfm_process() {
  bool ready = true;
  while (true) {
    ready_ch.write(ready);
    std::cout << sc_time_stamp() << ": reads from signal channel, valid =" << valid_if->read() << ", data =" << data_if->read() << std::endl;
    wait();
  }
}