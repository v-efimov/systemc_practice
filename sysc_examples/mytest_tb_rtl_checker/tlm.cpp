#include "systemc.h"
#include "tlm.h"

void TLM::tlm_process() {
  int readinputvalue;
  while (true) {
    readinputvalue = input_ch.read();
    std::cout << sc_time_stamp() << " Hey I'm the model, I received the value from input: " << readinputvalue << std::endl;
  }
}
