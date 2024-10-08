#ifndef TLM_H
#define TLM_H

SC_MODULE (TLM) {
  sc_export<sc_fifo<int>> PIPES_p;
  sc_fifo<int> PIPES_ch;
  sc_export<sc_fifo<int>> output_p;
  sc_fifo<int> output_ch;

  SC_CTOR(TLM) {
    PIPES_p(PIPES_ch);
    output_p(output_ch);
    SC_THREAD(tlm_process);
  }
  ~TLM() {
    std::cout << sc_time_stamp() << ": Cleanup: destructor" << std::endl;
  }
  void tlm_process();
};

#endif