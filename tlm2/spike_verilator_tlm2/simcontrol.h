#ifndef SIMCNTRL_H
#define SIMCNTRL_H


struct simcontrol_module: sc_module {
  sc_port<sc_signal_inout_if<bool>> stopsim_port;
  sc_port<sc_signal_inout_if<bool>> rstn_port;

  SC_CTOR(simcontrol_module) {
    SC_THREAD(simcontrol_thread);
  }
  ~simcontrol_module() {
    std::cout << sc_time_stamp() << ": Cleanup: destructor" << std::endl;
  }
  void start_of_simulation(void) override;
  void simcontrol_thread();
};

#endif