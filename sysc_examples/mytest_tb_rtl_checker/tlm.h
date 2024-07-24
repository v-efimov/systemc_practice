#ifndef TLM_H
#define TLM_H

SC_MODULE (TLM) {
  sc_export<sc_fifo<int>> input_p;
  sc_fifo<int> input_ch;
  sc_export<sc_fifo<int>> output_p;
  sc_fifo<int> output_ch;

  SC_CTOR(TLM) {
    input_p(input_ch);
    output_p(output_ch);
    SC_THREAD(tlm_process);
  }
  ~TLM() {
    std::cout << sc_time_stamp() << ": Cleanup: destructor" << std::endl;
  }
  void tlm_process();
};

#endif