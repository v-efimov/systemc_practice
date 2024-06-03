#include "systemc.h"
#include "module2.h"

void MODULE2::reader() {
  while (true) {
    std::cout << sc_time_stamp() << ": reads from fifo channel, val=" << p->read() << std::endl;
  }
}