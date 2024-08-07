#ifndef BFM_H
#define BFM_H

SC_MODULE(BFM) {
  sc_port<sc_signal_in_if<bool>> clk_port;
  sc_port<sc_signal_in_if<bool>> AXIS_valid_port;
  sc_port<sc_signal_in_if<std::uint32_t>> AXIS_data_port;
  sc_export<sc_signal<bool>> AXIS_ready_ptr;
  sc_signal<bool> AXIS_ready_ch;
  sc_export<sc_signal<bool>> AXIM_valid_ptr;
  sc_signal<bool> AXIM_valid_ch;
  sc_export<sc_signal<std::uint32_t>> AXIM_data_ptr;
  sc_signal<std::uint32_t> AXIM_data_ch;
  sc_port<sc_signal_in_if<bool>> AXIM_ready_port;
  sc_port<sc_fifo_out_if<std::uint32_t>> PIPEM_port;
  sc_port<sc_fifo_in_if<std::uint32_t>> PIPES_port;
  SC_CTOR(BFM) {
    AXIS_ready_ptr(AXIS_ready_ch);
    AXIM_valid_ptr(AXIM_valid_ch);
    AXIM_data_ptr(AXIM_data_ch);
    SC_THREAD(bfm_process_axis);
    sensitive << clk_port;
    dont_initialize();
    SC_THREAD(bfm_process_axim);
    sensitive << clk_port;
    dont_initialize();
  }
  ~BFM() {
    std::cout << sc_time_stamp() << ": Cleanup: destructor" << std::endl;
  }
  void bfm_process_axis();
  void bfm_process_axim();
};

#endif