#include "systemc.h"
#include "module2.h"

void MODULE2::reader() {
  CUSTOMIZED_TYPE readvalue;
  while (true) {
    readvalue = p->read();
    std::cout << sc_time_stamp() << ": reads from fifo channel, valX= " << readvalue.x << std::endl;
    std::cout << sc_time_stamp() << ": reads from fifo channel, valY= " << readvalue.y << std::endl;
  }
}