#include "systemc.h"
#include "uncore.h"

void uncore_module::b_transport (tlm::tlm_generic_payload& trans, sc_time& delay) {

        int command = 0;
        //check tlm_generic_payload

        tlm::tlm_command cmd = trans.get_command();

        std::uint32_t addr = trans.get_address();
        uint32_t* data = reinterpret_cast<uint32_t*>(trans.get_data_ptr());
        size_t length = trans.get_data_length() / sizeof(uint32_t);

        ///We can do input checks here and if something wrong dont proceed but set response status
        /*
        if (adr < 0 ) {
            trans.set_response_status (tlm::TLM_ADDRESS_ERROR_RESPONSE);
            return;
        }
        if (byt != 0 ) {
            trans.set_response_status (tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE);
            return;
        }
        if (len > 4 || wid < len ) {
            trans.set_response_status (tlm::TLM_BURST_ERROR_RESPONSE);
            return;
        }
        */

        if (trans.is_read()) {
            command = 0;
            cout << "TLM_rread_COMMAND" << endl;
        }
        if (trans.is_write()) {
            command = 1;
            cout << "TLM_wwrite_COMMAND" << endl;
        }

        cout << "ADDR: " << addr << endl;
        cout << "DATA: " << data[0] << endl;
        cout << "LENGTH: " << length << endl;

        for (int i=0; i<length; i++) {

        sc_bv<32> bv;
        bv.range(31, 28) = command;
        bv.range(27, 24) = 0;
        bv.range(23, 16) = addr + i;
        bv.range(15, 0) =  data[i];
        std::uint32_t req_vec = bv.to_uint();

        req_PIPE_ch.write(req_vec);
        resp_vec = resp_PIPE_ch.read();

        sc_bv<32> bv_vec = resp_vec;

        data[i] = bv_vec.range(15, 0).to_uint();

        }

        wait(delay);
        delay = SC_ZERO_TIME;    //we just set delay to zero as it is recommended by TLM2.0, this has no effect.

        trans.set_response_status(tlm::TLM_OK_RESPONSE);
}
