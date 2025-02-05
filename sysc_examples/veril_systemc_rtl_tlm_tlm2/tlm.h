#ifndef TLM_H
#define TLM_H

#include "initiator.h"

SC_MODULE (tlm_module) {
  sc_export<sc_fifo<std::uint32_t>> PIPES_ptr;
  sc_fifo<std::uint32_t> PIPES_ch;
  sc_export<sc_fifo<std::uint32_t>> PIPEM_ptr;
  sc_fifo<std::uint32_t> PIPEM_ch;
  sc_port<sc_fifo_out_if<std::uint32_t>> PIPESVA_port;

  initiator_module *initiator;

  SC_CTOR(tlm_module) : PIPES_ch(4), PIPEM_ch(4) {

    initiator = new initiator_module("initiator");

    PIPES_ptr(PIPES_ch);
    PIPEM_ptr(PIPEM_ch);
    PIPESVA_port(initiator->PIPESVA_ptr);

    SC_THREAD(tlm_thread);
  }
  ~tlm_module() {
    std::cout << sc_time_stamp() << ": Cleanup: destructor" << std::endl;
  }
  void tlm_thread();
};

#endif