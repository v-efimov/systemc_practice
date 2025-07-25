#ifndef SPIKE_H
#define SPIKE_H

#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"

struct spike_module: sc_module {
  sc_port<sc_signal_inout_if<bool>> stopsim_port;

  //We make transport socket and we reuse the same tlm_generic_payload to pass data over that socket
  tlm_utils::simple_initiator_socket<spike_module> spike_uncore_socket;
  std::unique_ptr<tlm::tlm_generic_payload> spike_uncore_trans;
  std::uint32_t data_array[1];      //ARRAY OF SIZE ONE ONLY IS CURRENTLY SUPPORTED BECAUSE INPUT DATA ACCEPTS ONLY ONE VALUE

  sc_time model_delay = sc_time(10, SC_NS);

  SC_CTOR(spike_module) : spike_uncore_socket("spike_uncore_socket"), spike_uncore_trans(std::make_unique<tlm::tlm_generic_payload>()), data_array{777} {
    SC_THREAD(spike_thread);
  }
  ~spike_module() {
    std::cout << sc_time_stamp() << ": Cleanup: destructor" << std::endl;
  }

  void start_of_simulation() override;
  void spike_thread();
};

#endif