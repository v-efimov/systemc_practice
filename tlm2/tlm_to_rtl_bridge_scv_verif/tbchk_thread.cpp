#include "systemc.h"
#include "testbench.h"

void testbench_module::tbchk_thread() {

  bool sim_test_ok = true;
  bool keep_sim = true;

  while (keep_sim) {
      resp_vec = resp_PIPE_ch.read();
      ref_vec = ref_PIPE_ch.read();
      tr_tocken = trtocken_PIPE_ch.read();
      cout << "HEY_I_GOT_THIS: " << resp_vec << "///" << ref_vec << endl;
      response.rtl_data_vector = resp_vec;
      response.model_data_vector = ref_vec;
      response.sync_structs_with_vectors();

      //Lets ask model
      if (response.model_data_struct.bus_signal == 1) {
          testbench_memmodel_trans->set_command(tlm::TLM_WRITE_COMMAND);
      } else {
          testbench_memmodel_trans->set_command(tlm::TLM_READ_COMMAND);
      }
      testbench_memmodel_trans->set_address(response.model_data_struct.bus_addr);
      data_array[0] = response.model_data_struct.bus_data;
      testbench_memmodel_trans->set_data_ptr( reinterpret_cast<unsigned char*>(data_array) );
      testbench_memmodel_trans->set_data_length(sizeof(data_array));
      testbench_memmodel_trans->set_streaming_width(sizeof(data_array));
      testbench_memmodel_trans->set_byte_enable_ptr(0);
      testbench_memmodel_trans->set_dmi_allowed(false);
      testbench_memmodel_trans->set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);

      model_delay = sc_time(99, SC_NS);
      testbench_memmodel_socket->b_transport(*testbench_memmodel_trans, model_delay);

      if ( testbench_memmodel_trans->get_response_status() != tlm::TLM_OK_RESPONSE ) {
          cout << "Transaction returned with error: " << testbench_memmodel_trans->get_response_status() << endl;
          sim_test_ok = false;
      } else {
          cout << "B_Transport_response_OK" << endl;
      }
      cout << "RESP_DATA: " << data_array[0] << endl;
      response.model_data_struct.bus_data = data_array[0];
      if (!response.compare_structs()) {
          sim_test_ok = false;
      };

      if (tr_tocken == 0) {
          keep_sim = false;
          stopsim_port->write(true);   //request to finish simulation
      } else {
          wait(long_delay);
      };
  }

  if (sim_test_ok) {
      cout << "SIMULATION_OK" << endl;
  } else {
      cout << "SIMULATION_ERROR!" << endl;
  }

}
