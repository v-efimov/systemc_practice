#ifndef TESTBENCH_H
#define TESTBENCH_H

////Instead of these includes try to use forward-declarations if possible
#include "scv.h"
#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "busstructs.h"

struct testbench_module: sc_module {
  sc_port<sc_signal_inout_if<bool>> stopsim_port;
  sc_export<sc_fifo<std::uint32_t>> req_PIPE;
  sc_fifo<std::uint32_t> req_PIPE_ch;
  sc_export<sc_fifo<std::uint32_t>> resp_PIPE;
  sc_fifo<std::uint32_t> resp_PIPE_ch;
  sc_fifo<std::uint32_t> ref_PIPE_ch;        //we pass the same value to RTL and to the checker so checker can request memory model
  sc_fifo<std::uint32_t> trtocken_PIPE_ch;   //this queue is to send hwo much checker shall be dalayed after processing the transaction

  std::uint32_t resp_vec, ref_vec, tr_tocken;

  scv_smart_ptr<data_t> smrtptr_data_t;
  stimulus_class<data_t, uint32_t> stimulus;
  response_class<data_t, uint32_t> response;

  //We make transport socket and we reuse the same tlm_generic_payload to pass data over that socket
  tlm_utils::simple_initiator_socket<testbench_module> testbench_memmodel_socket;
  std::unique_ptr<tlm::tlm_generic_payload> testbench_memmodel_trans;
  std::uint32_t data_array[1];      //ARRAY OF SIZE ONE ONLY IS CURRENTLY SUPPORTED BECAUSE INPUT DATA ACCEPTS ONLY ONE VALUE

  sc_time no_delay = sc_time(0, SC_NS);
  sc_time long_delay = sc_time(100, SC_NS);
  sc_time random_delay = sc_time(50, SC_NS);
  sc_time model_delay = sc_time(0, SC_NS);

  SC_CTOR(testbench_module) : req_PIPE_ch(4), resp_PIPE_ch(4), ref_PIPE_ch(4), trtocken_PIPE_ch(40), smrtptr_data_t("data"), stimulus(smrtptr_data_t), response(), testbench_memmodel_trans(std::make_unique<tlm::tlm_generic_payload>()), data_array{777} {
    req_PIPE(req_PIPE_ch);
    resp_PIPE(resp_PIPE_ch);
    SC_THREAD(tbgen_thread);
    SC_THREAD(tbchk_thread);
  }

  ~testbench_module() {
    std::cout << sc_time_stamp() << ": Cleanup: destructor" << std::endl;
  }

  void start_of_simulation() override {
    stopsim_port->write(false);
    std::cout << sc_time_stamp() << "Initial (RESET_ON)" << std::endl;
  }

  void tbgen_thread();
  void tbchk_thread();

};

#endif
