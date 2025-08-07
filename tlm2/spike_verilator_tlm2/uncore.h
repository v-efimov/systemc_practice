#ifndef UNCORE_H
#define UNCORE_H

////Instead of these includes try to use forward-declarations if possible
#include "tlm.h"
#include "tlm_utils/simple_target_socket.h"

struct uncore_module: sc_module {
  sc_port<sc_signal_inout_if<bool>> stopsim_port;
  sc_export<sc_fifo<std::uint32_t>> req_PIPE;
  sc_fifo<std::uint32_t> req_PIPE_ch;
  sc_export<sc_fifo<std::uint32_t>> resp_PIPE;
  sc_fifo<std::uint32_t> resp_PIPE_ch;

  tlm_utils::simple_target_socket<uncore_module> spike_uncore_socket;

  std::uint32_t resp_vec;

  SC_CTOR(uncore_module) : req_PIPE_ch(4), resp_PIPE_ch(4), spike_uncore_socket("spike_uncore_socket") {
    req_PIPE(req_PIPE_ch);
    resp_PIPE(resp_PIPE_ch);
    spike_uncore_socket.register_b_transport(this, &uncore_module::b_transport);
  }

  ~uncore_module() {
    std::cout << sc_time_stamp() << ": Cleanup: destructor" << std::endl;
  }

  void start_of_simulation() override;

  void b_transport (tlm::tlm_generic_payload& trans, sc_time& delay);

};

#endif