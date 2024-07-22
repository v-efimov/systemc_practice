#include "systemc.h"
#include "bfm.h"

void BFM::reader() {
  while (true) {
    std::cout << sc_time_stamp() << ": reads from signal channel, val=" << data_if->read() << std::endl;
    wait();
  }
}