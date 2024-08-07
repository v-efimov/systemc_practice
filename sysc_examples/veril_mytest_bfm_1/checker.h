#ifndef CHECKER_H
#define CHECKER_H

SC_MODULE(CHECKER) {

  sc_port<sc_signal_in_if<bool>> clk_port;
  sc_port<sc_signal_in_if<bool>> AXIS_valid_port;
  sc_port<sc_signal_in_if<std::uint32_t>> AXIS_data_port;
  sc_export<sc_signal<bool>> AXIS_ready_ptr;
  sc_signal<bool> AXIS_ready_ch;

  SC_CTOR(CHECKER) {
    AXIS_ready_ptr(AXIS_ready_ch);
    SC_THREAD(checker_process);
    sensitive << clk_port;
    dont_initialize();
  }
  ~CHECKER() {
    std::cout << sc_time_stamp() << ": Cleanup: destructor" << std::endl;
  }
  void checker_process();
};

#endif