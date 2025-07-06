#include "systemc.h"
#include "pins2queue.h"

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
      //Write data into expansion REGs if handshake happens but no free place in PIPEM_FIFO
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
