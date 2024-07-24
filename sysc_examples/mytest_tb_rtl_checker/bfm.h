#ifndef BFM_H
#define BFM_H

SC_MODULE(BFM) {
  sc_port<sc_signal_in_if<bool>> clk;
  sc_port<sc_signal_in_if<bool>> valid_if;
  sc_port<sc_signal_in_if<int>> data_if;
  sc_export<sc_signal<bool>> ready_p;
  sc_signal<bool> ready_ch;
  sc_port<sc_fifo_out_if<int>> input_if;
  SC_CTOR(BFM) {
    ready_p(ready_ch);
    SC_THREAD(bfm_process);
    sensitive << clk;
    dont_initialize();
  }
  ~BFM() {
    std::cout << sc_time_stamp() << ": Cleanup: destructor" << std::endl;
  }
  void bfm_process();
};

#endif