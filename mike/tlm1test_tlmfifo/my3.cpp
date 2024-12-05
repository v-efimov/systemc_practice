

#include <systemc> // include the systemC header file
using namespace sc_core; // use namespace

#include "tlm.h"


typedef struct axis_bus_s {
   bool     last;
   uint32_t data;
} axis_bus_t;

tlm::tlm_fifo<axis_bus_t> fifo;

SC_MODULE(MOD_TX) {

   SC_CTOR(MOD_TX) {
      SC_THREAD(thread_tx);
   }

   void thread_tx() {
      axis_bus_t  tx_val;

      tx_val.last = false;
      tx_val.data = 0;

      while (true) {
         std::cout << "thread_tx triggered @ " << sc_time_stamp()  << std::endl;

         tx_val.data++;
         tx_val.last = (tx_val.data % 3 == 2);

         fifo.put(tx_val);
         wait (1, SC_SEC);
      }
   }

};

SC_MODULE(MOD_RX) {

   SC_CTOR(MOD_RX) {
      SC_THREAD(thread_rx);
   }

   void thread_rx() {

      axis_bus_t  rx_val;

      while (true) {
         rx_val = fifo.get();
         std::cout << "thread_rx triggered @ " << sc_time_stamp() << " val = " << rx_val.data << " last = " << rx_val.last << std::endl;
      }
   }

};

int sc_main(int, char*[]) {
   MOD_TX mod_tx("mod_tx");
   MOD_RX mod_rx("mod_rx");

   std::cout << "start @ " << sc_time_stamp()  << std::endl;

   sc_start (11, SC_SEC);

   std::cout << "finish @ " << sc_time_stamp()  << std::endl;

   return 0;
}
