#ifndef TB_H
#define TB_H

SC_MODULE (TB) {
  sc_port<sc_signal_in_if<bool>> clk_port;
  sc_export<sc_signal<bool>> AXIM_valid_ptr;
  sc_signal<bool> AXIM_valid_ch;
  sc_export<sc_signal<std::uint32_t>> AXIM_data_ptr;
  sc_signal<std::uint32_t> AXIM_data_ch;
  sc_port<sc_signal_in_if<bool>> AXIM_ready_port;

  SC_CTOR(TB) {
    AXIM_data_ptr(AXIM_data_ch);
    AXIM_valid_ptr(AXIM_valid_ch);
    SC_THREAD(tb_process);
    sensitive << clk_port;
    dont_initialize();
  }
  ~TB() {
    std::cout << sc_time_stamp() << ": Cleanup: destructor" << std::endl;
  }
  void tb_process();
};

#endif
