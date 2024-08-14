#ifndef BFM_H
#define BFM_H

SC_MODULE(BFM) {
  sc_port<sc_signal_in_if<bool>> clk;
  sc_port<sc_signal_in_if<bool>> AXIS_valid_if;
  sc_port<sc_signal_in_if<int>> AXIS_data_if;
  sc_export<sc_signal<bool>> AXIS_ready_p;
  sc_signal<bool> AXIS_ready_ch;
  sc_export<sc_signal<bool>> AXIM_valid_p;
  sc_signal<bool> AXIM_valid_ch;
  sc_export<sc_signal<int>> AXIM_data_p;
  sc_signal<int> AXIM_data_ch;
  sc_port<sc_signal_in_if<bool>> AXIM_ready_if;
  sc_port<sc_fifo_out_if<int>> PIPEM_if;
  sc_port<sc_fifo_in_if<int>> PIPES_if;
  SC_CTOR(BFM) {
    AXIS_ready_p(AXIS_ready_ch);
    AXIM_valid_p(AXIM_valid_ch);
    AXIM_data_p(AXIM_data_ch);
    SC_THREAD(bfm_process_axis);
    sensitive << clk;
    dont_initialize();
    SC_THREAD(bfm_process_axim);
    sensitive << clk;
    dont_initialize();
  }
  ~BFM() {
    std::cout << sc_time_stamp() << ": Cleanup: destructor" << std::endl;
  }
  void bfm_process_axis();
  void bfm_process_axim();
};

#endif