// SystemC global header
#include <systemc.h>

using namespace sc_core;


SC_MODULE (MODULE1) {
  sc_export<sc_fifo<int>> p;
  sc_fifo<int> s;

  SC_CTOR(MODULE1) {
    p(s);
    SC_THREAD(writer);
  }


void writer() {
  int val = 1;
  while (true) {
    s.write(val++);
    wait(SC_ZERO_TIME);
  }
 }
};


SC_MODULE(MODULE2) {
  sc_port<sc_fifo_in_if<int>> p;
  SC_CTOR(MODULE2) {
    SC_THREAD(reader);
  }

void reader() {
  while (true) {
    std::cout << sc_time_stamp() << ": reads from fifo channel, val=" << p->read() << std::endl;
  }
 }
};


int sc_main(int, char* []) {
    MODULE1 module1("module1");
    MODULE2 module2("module2");
    module2.p(module1.p);

    sc_start(10, SC_SEC);
    return 0;
}
