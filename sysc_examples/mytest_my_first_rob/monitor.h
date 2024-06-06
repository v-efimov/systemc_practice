#ifndef MONITOR_H
#define MONITOR_H

SC_MODULE(MONITOR) {
  sc_port<sc_fifo_in_if<int>> p;
  SC_CTOR(MONITOR) {
    SC_THREAD(monitor);
  }
  ~MONITOR() {
    std::cout << sc_time_stamp() << ": Cleanup: destructor" << std::endl;
  }
  void monitor();
};

#endif