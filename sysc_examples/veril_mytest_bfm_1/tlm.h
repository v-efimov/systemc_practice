#ifndef TLM_H
#define TLM_H

SC_MODULE (TLM) {
  sc_export<sc_fifo<std::uint32_t>> PIPES_ptr;
  sc_fifo<std::uint32_t> PIPES_ch;
  sc_export<sc_fifo<std::uint32_t>> PIPEM_ptr;
  sc_fifo<std::uint32_t> PIPEM_ch;

  SC_CTOR(TLM) : PIPES_ch(4), PIPEM_ch(4) {
    PIPES_ptr(PIPES_ch);
    PIPEM_ptr(PIPEM_ch);
    SC_THREAD(tlm_process);
  }
  ~TLM() {
    std::cout << sc_time_stamp() << ": Cleanup: destructor" << std::endl;
  }
  void tlm_process();
};

#endif