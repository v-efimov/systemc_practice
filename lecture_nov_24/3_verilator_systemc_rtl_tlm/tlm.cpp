#include "systemc.h"
#include "tlm.h"

void tlm_module::tlm_thread() {
  std::uint32_t readinputvalue;
  int simple_data = 0;
  while (true) {
    readinputvalue = PIPES_ch.read();
    if (readinputvalue % 2) {
        std::cout << sc_time_stamp() << " The data is odd, write once: " << readinputvalue << std::endl;
        PIPEM_ch.write(readinputvalue);
        }
    else
        {
        std::cout << sc_time_stamp() << " The data is even, write twice: " << readinputvalue << std::endl;
        PIPEM_ch.write(readinputvalue*readinputvalue);
        PIPEM_ch.write(readinputvalue*readinputvalue);
        }
  }
}
