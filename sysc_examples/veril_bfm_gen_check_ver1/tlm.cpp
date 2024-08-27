#include "systemc.h"
#include "tlm.h"

void TLM::tlm_process() {
  std::uint32_t readinputvalue;
  while (true) {
    readinputvalue = PIPES_ch.read();
    std::cout << sc_time_stamp() << " Hey I'm the model, I RECEIVED the value from input: " << readinputvalue << std::endl;
    PIPEM_ch.write(readinputvalue);
    std::cout << sc_time_stamp() << " Hey I'm the model, I WROTE the value: " << readinputvalue << std::endl;
    PIPEM_ch.write(readinputvalue*readinputvalue);
    std::cout << sc_time_stamp() << " Hey I'm the model, I WROTE the value: " << readinputvalue*readinputvalue << std::endl;
  }
}
