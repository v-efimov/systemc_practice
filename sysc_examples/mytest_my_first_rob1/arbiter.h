#ifndef ARBITER_H
#define ARBITER_H

SC_MODULE(ARBITER) {
  sc_port<sc_fifo_in_if<int>> inp1, inp2;
  sc_port<sc_fifo_out_if<int>> outp1;
  SC_CTOR(ARBITER) {
    SC_THREAD(arbiter);
  }
  ~ARBITER() {
    std::cout << sc_time_stamp() << ": Cleanup: destructor" << std::endl;
  }
  void arbiter();
};

#endif
