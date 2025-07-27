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

  sc_time gen_delay, chk_delay, model_delay;

  const sc_time MODEL_DELAY = sc_time(0, SC_NS);
  const int FINAL_TRANSFER_TOCKEN = 99;


  SC_CTOR(testbench_module) : req_PIPE_ch(1), resp_PIPE_ch(1), ref_PIPE_ch(100), trtocken_PIPE_ch(100), smrtptr_data_t("data"), stimulus(smrtptr_data_t), response(), testbench_memmodel_trans(std::make_unique<tlm::tlm_generic_payload>()), data_array{777} {
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
    std::cout << sc_time_stamp() << "Initial (STOPSIM_OFF)" << std::endl;
  }

  void get_thread_delay (sc_time &delay, int mode) {
   switch (mode) {
        case 0: {
            delay = sc_time(0, SC_NS);              //Fast mode, zero delay
            break;
        }
        case 1: {
            int rand_ns = std::rand() % 3000 + 0;
            delay = sc_time(rand_ns, SC_NS);        //Random delay
            break;
        }
        case 2: {
            delay = sc_time(3000, SC_NS);           //Slow mode, high delay
            break;
        }
        default: {
            delay = sc_time(0, SC_NS);;
        }
    }
}

  void tbgen_thread();
  void tbchk_thread();

};

#endif
