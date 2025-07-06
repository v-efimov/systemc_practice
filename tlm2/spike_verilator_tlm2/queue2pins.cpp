#include "systemc.h"
#include "queue2pins.h"

template struct queue2pins_module<std::uint32_t>;

template <typename T>
void queue2pins_module<T>::queue2pins_method() {
    if (rstn_port->read() == false) {
       down_valid->write(false);
    }
    else
    {
      //we are comparing pre-clock value here.
      //pipeline drives if ~down_valid or down_ready
      if (down_valid->read() == false || down_ready->read() == true) {
        if (up_PIPE->nb_read(ReadValueFromUpPIPE) == true) {
          down_valid->write(true);
          down_data->write(ReadValueFromUpPIPE);
        }
        else
        {
          down_valid->write(false);
          //we only modify value of valid and don't modify value of data signal
        }
      }
    }
}
