#ifndef BFM_H
#define BFM_H

SC_MODULE(bfm_module) {
  sc_in_clk clk_port;
  sc_port<sc_signal_in_if<bool>> rst_port;
  sc_port<sc_signal_in_if<bool>> AXIS_valid_port;
  sc_port<sc_signal_in_if<std::uint32_t>> AXIS_data_port;
  sc_port<sc_signal_inout_if<bool>> AXIS_ready_port;
  sc_port<sc_signal_inout_if<bool>> AXIM_valid_port;
  sc_port<sc_signal_out_if<std::uint32_t>> AXIM_data_port;
  sc_port<sc_signal_in_if<bool>> AXIM_ready_port;
  sc_port<sc_fifo_out_if<std::uint32_t>> PIPEM_port;
  sc_port<sc_fifo_in_if<std::uint32_t>> PIPES_port;
  sc_signal<std::uint32_t> REG;
  sc_signal<bool> state;
  enum BufState { BYPASS=false, SKID=true };
  uint32_t ReadValueFromPIPES;

  SC_CTOR(bfm_module) {
    SC_METHOD(bfm_axis_method);
    sensitive << clk_port.pos();
    SC_METHOD(bfm_axim_method);
    sensitive << clk_port.pos();
  }
  ~bfm_module() {
    std::cout << sc_time_stamp() << ": Cleanup: destructor" << std::endl;
  }
  void bfm_axis_method();
  void bfm_axim_method();
};

#endif