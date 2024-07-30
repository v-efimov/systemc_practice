#ifndef CHECKER_H
#define CHECKER_H

SC_MODULE(CHECKER) {
  
  sc_port<sc_signal_in_if<bool>> clk;
  sc_port<sc_signal_in_if<bool>> AXIS_valid_if;
  sc_port<sc_signal_in_if<int>> AXIS_data_if;
  sc_export<sc_signal<bool>> AXIS_ready_p;
  sc_signal<bool> AXIS_ready_ch;

  SC_CTOR(CHECKER) {
    AXIS_ready_p(AXIS_ready_ch);
    SC_THREAD(checker_process);
    sensitive << clk;
    dont_initialize();
  }
  ~CHECKER() {
    std::cout << sc_time_stamp() << ": Cleanup: destructor" << std::endl;
  }
  void checker_process();
};

#endif