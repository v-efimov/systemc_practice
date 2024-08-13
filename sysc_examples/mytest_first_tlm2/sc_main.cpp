#define SC_INCLUDE_DYNAMIC_PROCESSES
// SystemC global header
#include <systemc.h>

using namespace sc_core;
using namespace sc_dt;
using namespace std;

#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/simple_target_socket.h"

struct Initiator: sc_module
{
  tlm_utils::simple_initiator_socket<Initiator> socket;
  SC_CTOR(Initiator) : socket("socket") {
 }
};

struct Memory: sc_module
{
  tlm_utils::simple_target_socket<Memory> socket;
  SC_CTOR(Memory) : socket("socket") {
  socket.register_b_transport(this, &Memory::b_transport);
 }

 virtual void b_transport(tlm::tlm_generic_payload& trans, sc_time& delay)
 {
   tlm:tlm_command cmd = trans.get_command();
   sc_dt::uint64   adr = trans.get_address() / 4;
   unsigned char*  ptr = trans.get_data_ptr();
   unsigned int    len = trans.get_data_lenght();
   unsigned char*  byt = trans.get_byte_enable_ptr();
   unsigned int    wid = trans.get_streaming_width();
   if (adr >= sc_dt::uint64(SIZE) || byt != 0 || len > 4 || wid < len)
     SC_REPORT_ERROR("TLM-2", "Target does not support generic payload transaction");
 }

};

SC_MODULE(Top)
{
 Initiator *initiator;
 Memory *memory;

 SC_CTOR(Top)
 {
   initiator = new Initiator("initiator");
   memory = new Memory ("memory");

   initiator->socket.bind(memory->socket);
 }
};

int sc_main(int argc, char* argv[])
}
 Top top("top");
 sc_start();
 return 0;
}
