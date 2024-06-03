#ifndef MODULE2_H
#define MODULE2_H

SC_MODULE(MODULE2) {
  sc_port<sc_fifo_in_if<int>> p;
  SC_CTOR(MODULE2) {
    SC_THREAD(reader);
  }
  ~MODULE2() {
    std::cout << sc_time_stamp() << ": Cleanup: destructor" << std::endl;
  }
  void reader();
};

#endif