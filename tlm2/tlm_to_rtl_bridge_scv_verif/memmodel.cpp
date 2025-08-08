#include "systemc.h"
#include "memmodel.h"

void memmodel_module::b_transport (tlm::tlm_generic_payload& trans, sc_time& delay) {

        std::uint32_t addr = trans.get_address();
        uint32_t* data = reinterpret_cast<uint32_t*>(trans.get_data_ptr());

        cout << "MODEL_CALLED: " << addr << endl;
        switch (trans.get_command()) {
            case tlm::TLM_READ_COMMAND: {
                data[0] = sparse_array[addr];
                cout << "TLM_read_COMMAND" << endl;
                break;
            }
            case tlm::TLM_WRITE_COMMAND: {
                sparse_array[addr] = data[0];
                cout << "TLM_write_COMMAND" << endl;
                break;
            }
            default: {
            cout << "TLM_not_read_nor_write" << endl;
            }
        }

        cout << "ADDR: " << addr << endl;
        cout << "DATA: " << data[0] << endl;

        cout << "DELDEL: " << delay << endl;
        wait(delay);
        delay = SC_ZERO_TIME;    //we just set delay to zero as it is recommended by TLM2.0, this has no effect.

        trans.set_response_status(tlm::TLM_OK_RESPONSE);
}
