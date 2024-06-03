#ifndef MODULE1_H
#define MODULE1_H

SC_MODULE (MODULE1) {
  sc_export<sc_fifo<int>> p;
  sc_fifo<int> s;

  SC_CTOR(MODULE1) {
    p(s);
    SC_THREAD(writer);
  }
  ~MODULE1() {
    std::cout << sc_time_stamp() << ": Cleanup: destructor" << std::endl;
  }
  void writer();
};

#endif