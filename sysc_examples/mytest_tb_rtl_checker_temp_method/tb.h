#ifndef TB_H
#define TB_H

SC_MODULE (TB) {
  sc_port<sc_signal_in_if<bool>> clk;
  sc_export<sc_signal<bool>> AXIM_valid_p;
  sc_signal<bool> AXIM_valid_ch;
  sc_export<sc_signal<int>> AXIM_data_p;
  sc_signal<int> AXIM_data_ch;
  sc_port<sc_signal_in_if<bool>> AXIM_ready_if;

  SC_CTOR(TB) {
    AXIM_data_p(AXIM_data_ch);
    AXIM_valid_p(AXIM_valid_ch);
    SC_THREAD(tb_process);
    sensitive << clk;
    dont_initialize();
  }
  ~TB() {
    std::cout << sc_time_stamp() << ": Cleanup: destructor" << std::endl;
  }
  void tb_process();
};

#endif
