#include "systemc.h"
#include "bfm.h"

void BFM::bfm_process_axim() {
    //we are comparing pre-clock value here.
    //pipeline drives if ~down_valid or down_ready
    if (AXIM_valid_ch.read() == false || AXIM_ready_if->read() == true) {
      if (PIPES_if->nb_read(ReadValueFromPIPES) == true) {
        AXIM_valid_ch.write(true);
        AXIM_data_ch.write(ReadValueFromPIPES);
      }
      else
      {
        AXIM_valid_ch.write(false);
        //we only modify value of valid and don't modify value of data signal
      }
    }
}
