#ifndef RESET_H
#define RESET_H


struct reset_module: sc_module {
  sc_port<sc_signal_inout_if<bool>> rstn_port;

  SC_CTOR(reset_module) {
    SC_THREAD(reset_thread);
  }
  ~reset_module() {
    std::cout << sc_time_stamp() << ": Cleanup: destructor" << std::endl;
  }

  void start_of_simulation() override;
  void reset_thread();
};

#endif