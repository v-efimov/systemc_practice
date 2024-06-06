#ifndef ARBITER_H
#define ARBITER_H

SC_MODULE(ARBITER) {
  sc_port<sc_fifo_in_if<int>> inp1, inp2;
  sc_fifo<int> so1;
  sc_export<sc_fifo<int>> outp1;
  SC_CTOR(ARBITER) {
    outp1(so1);
    SC_THREAD(arbiter);
  }
  ~ARBITER() {
    std::cout << sc_time_stamp() << ": Cleanup: destructor" << std::endl;
  }
  void arbiter();
};

#endif
