#ifndef MEMMODEL_H
#define MEMMODEL_H

////Instead of these includes try to use forward-declarations if possible
#include "tlm.h"
#include "tlm_utils/simple_target_socket.h"
#include "scv.h"

struct memmodel_module: sc_module {

  tlm_utils::simple_target_socket<memmodel_module> testbench_memmodel_socket;
  scv_sparse_array<unsigned, unsigned> sparse_array;

  SC_CTOR(memmodel_module, unsigned mem_up_bound) : testbench_memmodel_socket("testbench_memmodel_socket"), sparse_array("mem",0,0,mem_up_bound) {
    testbench_memmodel_socket.register_b_transport(this, &memmodel_module::b_transport);
  }

  ~memmodel_module() {
    std::cout << sc_time_stamp() << ": Cleanup: destructor" << std::endl;
  }


  void b_transport (tlm::tlm_generic_payload& trans, sc_time& delay);

};

#endif