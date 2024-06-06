#ifndef GENERATOR_H
#define GENERATOR_H

SC_MODULE (GENERATOR) {
  sc_export<sc_fifo<int>> p1, p2;
  sc_fifo<int> s1, s2;

  SC_CTOR(GENERATOR) {
    p1(s1);
    p2(s2);
    SC_THREAD(generator);
  }
  ~GENERATOR() {
    std::cout << sc_time_stamp() << ": Cleanup: destructor" << std::endl;
  }
  void generator();
};

#endif