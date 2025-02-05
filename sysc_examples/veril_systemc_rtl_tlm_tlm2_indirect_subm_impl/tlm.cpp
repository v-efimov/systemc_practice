#include "systemc.h"
#include "tlm.h"
#include "initiator.h"

SC_HAS_PROCESS(tlm_module);

tlm_module::tlm_module(sc_module_name nm) : sc_module(nm), PIPES_ch(4), PIPEM_ch(4) {

    initiator = new initiator_module("initiator");

    PIPES_ptr(PIPES_ch);
    PIPEM_ptr(PIPEM_ch);
    PIPESVA_port(initiator->PIPESVA_ptr);

    SC_THREAD(tlm_thread);
}

tlm_module::~tlm_module() {
    std::cout << sc_time_stamp() << ": Cleanup: destructor" << std::endl;
}

void tlm_module::tlm_thread() {
  std::uint32_t readinputvalue;
  while (true) {
    readinputvalue = PIPES_ch.read();
    if (readinputvalue % 2) {
        std::cout << sc_time_stamp() << " The data is odd, write once: " << readinputvalue << std::endl;
        PIPEM_ch.write(readinputvalue);
        PIPESVA_port->write(readinputvalue);
        }
    else
        {
        std::cout << sc_time_stamp() << " The data is even, write twice: " << readinputvalue << std::endl;
        PIPEM_ch.write(readinputvalue*readinputvalue);
        PIPEM_ch.write(readinputvalue*readinputvalue);
        }
  }
}
