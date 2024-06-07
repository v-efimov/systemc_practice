#ifndef MONITOR_H
#define MONITOR_H

SC_MODULE(MONITOR) {
  sc_fifo<int> so1;
  sc_export<sc_fifo<int>> inp1;
  SC_CTOR(MONITOR) {
    inp1(so1);
    SC_THREAD(monitor);
  }
  ~MONITOR() {
    std::cout << sc_time_stamp() << ": Cleanup: destructor" << std::endl;
  }
  void monitor();
};

#endif