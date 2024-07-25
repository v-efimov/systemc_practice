#ifndef BFM_H
#define BFM_H

SC_MODULE(BFM) {
  sc_port<sc_signal_in_if<bool>> clk;
  sc_port<sc_signal_in_if<bool>> AXIS_valid_if;
  sc_port<sc_signal_in_if<int>> AXIS_data_if;
  sc_export<sc_signal<bool>> AXIS_ready_p;
  sc_signal<bool> AXIS_ready_ch;
  sc_port<sc_fifo_out_if<int>> PIPEM_if;
  SC_CTOR(BFM) {
    AXIS_ready_p(AXIS_ready_ch);
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