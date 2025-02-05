#ifndef TLM_H
#define TLM_H

struct initiator_module;

SC_MODULE (tlm_module) {
  sc_export<sc_fifo<std::uint32_t>> PIPES_ptr;
  sc_fifo<std::uint32_t> PIPES_ch;
  sc_export<sc_fifo<std::uint32_t>> PIPEM_ptr;
  sc_fifo<std::uint32_t> PIPEM_ch;
  sc_port<sc_fifo_out_if<std::uint32_t>> PIPESVA_port;

  initiator_module *initiator;

  tlm_module(sc_module_name nm);

  ~tlm_module();
  void tlm_thread();
};

#endif