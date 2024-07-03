#ifndef MODULE1_H
#define MODULE1_H

#include "mytype.h"

SC_MODULE (MODULE1) {
  sc_export<sc_fifo<CUSTOMIZED_TYPE>> p;
  sc_fifo<CUSTOMIZED_TYPE> s;

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