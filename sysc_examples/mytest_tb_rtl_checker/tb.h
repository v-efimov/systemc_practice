#ifndef TB_H
#define TB_H

SC_MODULE (TB) {
  sc_port<sc_signal_in_if<bool>> clk;
  sc_export<sc_signal<bool>> valid_p;
  sc_signal<bool> valid_ch;
  sc_export<sc_signal<int>> data_p;
  sc_signal<int> data_ch;

  SC_CTOR(TB) {
    data_p(data_ch);
    valid_p(valid_ch);
    SC_THREAD(writer);
    sensitive << clk;
    dont_initialize();
  }
  ~TB() {
    std::cout << sc_time_stamp() << ": Cleanup: destructor" << std::endl;
  }
  void writer();
};

#endif
