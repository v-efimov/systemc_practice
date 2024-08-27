#ifndef BFM_H
#define BFM_H

SC_MODULE(BFM) {
  sc_port<sc_signal_in_if<bool>> clk_port;
  sc_port<sc_signal_in_if<bool>> rst_port;
  sc_port<sc_signal_in_if<bool>> AXIS_valid_port;
  sc_port<sc_signal_in_if<std::uint32_t>> AXIS_data_port;
  sc_port<sc_signal_inout_if<bool>> AXIS_ready_port;
  sc_port<sc_signal_inout_if<bool>> AXIM_valid_port;
  sc_port<sc_signal_out_if<std::uint32_t>> AXIM_data_port;
  sc_port<sc_signal_in_if<bool>> AXIM_ready_port;
  sc_port<sc_fifo_out_if<std::uint32_t>> PIPEM_port;
  sc_port<sc_fifo_in_if<std::uint32_t>> PIPES_port;
  uint32_t REG;
  enum BufState { BYPASS, SKID } state;
  uint32_t ReadValueFromPIPES;

  SC_CTOR(BFM) {
    state = BYPASS;
    //AXIS_ready_port->write(true);
    SC_METHOD(bfm_process_axis);
    sensitive << clk_port;
    SC_METHOD(bfm_process_axim);
    sensitive << clk_port;
  }
  ~BFM() {
    std::cout << sc_time_stamp() << ": Cleanup: destructor" << std::endl;
  }
  void bfm_process_axis();
  void bfm_process_axim();
};

#endif