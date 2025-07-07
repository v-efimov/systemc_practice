#include "systemc.h"
#include "spike.h"

void spike_module::spike_thread() {
  sc_time delay = sc_time(10, SC_NS);

  bool input_sanity_check_ok = false;
  int inputaddress;
  int inputdata;
  int inputoperation;

  while (true) {

    input_sanity_check_ok = false;
    do {
        std::cout << "Curr simtime:" <<  sc_time_stamp() << ", provide \"ADDRESS    DATA    OPERATION(1-write/2-read)\" separated by spaces: ";
        std::cin >> inputaddress >> inputdata >> inputoperation;
        if (!std::cin.fail() && inputoperation >= 1 && inputoperation <= 2) {
            input_sanity_check_ok = true;
            std::cout << "Input accepted" << std::endl;
        } else {
            std::cin.clear(); // Reset error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //Discard input
            std::cout << "Oops! That wasn't valid. Try again. \n";
        }
    } while (!input_sanity_check_ok);

    if (inputoperation == 1) {
        spike_uncore_trans->set_command(tlm::TLM_WRITE_COMMAND);
    } else {
        spike_uncore_trans->set_command(tlm::TLM_READ_COMMAND);
    }
    spike_uncore_trans->set_address(inputaddress);
    data_array[0] = inputdata;
    spike_uncore_trans->set_data_ptr( reinterpret_cast<unsigned char*>(data_array) );
    spike_uncore_trans->set_data_length(sizeof(data_array));
    spike_uncore_trans->set_streaming_width(sizeof(data_array));
    spike_uncore_trans->set_byte_enable_ptr(0);
    spike_uncore_trans->set_dmi_allowed(false);
    spike_uncore_trans->set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);

    spike_uncore_socket->b_transport(*spike_uncore_trans, delay);

    if ( spike_uncore_trans->get_response_status() != tlm::TLM_OK_RESPONSE ) {
        cout << "Transaction returned with error: " << spike_uncore_trans->get_response_status() << endl;
    } else {
        cout << "B_Transport_response_OK" << endl;
    }

    cout << "RESP_DATA: " << data_array[0] << endl;

  }
}
