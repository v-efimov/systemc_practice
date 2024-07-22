#ifndef BFM_H
#define BFM_H

SC_MODULE(BFM) {
  sc_port<sc_signal_in_if<bool>> clk;
  sc_port<sc_signal_in_if<int>> data_if;
  SC_CTOR(BFM) {
    SC_THREAD(reader);
    sensitive << clk;
    dont_initialize();
  }
  ~BFM() {
    std::cout << sc_time_stamp() << ": Cleanup: destructor" << std::endl;
  }
  void reader();
};

#endif