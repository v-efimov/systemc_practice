#ifndef INITIATOR_H
#define INITIATOR_H


struct initiator_module: sc_module
{
  sc_export<sc_fifo<std::uint32_t>> PIPESVA_ptr;
  sc_fifo<std::uint32_t> PIPESVA_ch;

  SC_CTOR(initiator_module) : PIPESVA_ch(4) {
      PIPESVA_ptr(PIPESVA_ch);
      SC_THREAD(initiator_thread);
    }

~initiator_module() {
    std::cout << sc_time_stamp() << ": Cleanup: destructor" << std::endl;
  }

void initiator_thread()
{
  std::uint32_t readinputvalue;
  while (true) {
    readinputvalue = PIPESVA_ch.read();
    std::cout << sc_time_stamp() << " HOHOHO-IM-INITIATOR_LLOK-WHAT-I-GOT: " << readinputvalue << std::endl;
  }
}

};

#endif

