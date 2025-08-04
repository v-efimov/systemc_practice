#include "systemc.h"
#include "pins2queue.h"

//NOTE: We use "skid-buffer" concept here because "skid_buffer" sets "up_ready" signal based on available space in the "REG" expansion register which is exclusive to this "skid_buffer" module.
//Alternative approach could be to check for available space in the "down_PIPE" which is "sc_fifo" and set "up_ready" signal if there is space in that "down_PIPE" fifo.
//Although latter approach is simpler is seems less preferable because "down_PIPE" is external to this module in ther words it is instantiated in a different module and terefore that "down_PIPE" may
//potentially have other writers to it not just the current module. This could result in a situation with "up_ready" being set to high at the local module but with no available space in "down_PIPE"
//and this may lead to the loss of data.

template struct pins2queue_module<std::uint32_t>;

template <typename T>
void pins2queue_module<T>::pins2queue_method() {
    if (rstn_port->read() == false) {
       state.write(BufState::BYPASS);
       up_ready->write(true);
    }
    else
    {
      //we are comparing pre-clock value here.
      //Write data into expansion REGs if handshake happens but no free place in down_PIPE fifo
      if (state.read() == BufState::BYPASS) {
          if (up_valid->read() == true && up_ready->read() == true) {
            if (down_PIPE->nb_write(up_data->read()) == false) {
                REG.write(up_data->read());
                state.write(BufState::SKID);
                up_ready->write(false);
            }
          }
      }
      else if (state.read() == BufState::SKID) {
          if (down_PIPE->nb_write(REG) == true) {
             state.write(BufState::BYPASS);
             up_ready->write(true);
          }
      }
      else {}
    }
}
