#ifndef TLMWRAP_H
#define TLMWRAP_H

#include "target.h"
#include "systemc.h"
using namespace sc_core;
using namespace sc_dt;
using namespace std;

#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"


SC_MODULE (TLMWRAP) {
  sc_export<sc_fifo<int>> PIPES_p;
  sc_fifo<int> PIPES_ch;
  sc_export<sc_fifo<int>> PIPEM_p;
  sc_fifo<int> PIPEM_ch;

  Memory    *memory;
  tlm_utils::simple_initiator_socket<TLMWRAP> socket;

  SC_CTOR(TLMWRAP) : PIPES_ch(4), PIPEM_ch(4), socket("socket") {
    //TLM-2 socket, defaults to 32-bits wide, base protocol
    PIPES_p(PIPES_ch);
    PIPEM_p(PIPEM_ch);
    //SC_THREAD(tlmwrap_process);
    // Instantiate components
    memory    = new Memory   ("memory");
    //Bind initiator socket to target socket
    socket.bind(memory->socket);
    SC_THREAD(thread_process);
  }
  ~TLMWRAP() {
    std::cout << sc_time_stamp() << ": Cleanup: destructor" << std::endl;
  }
  //void tlmwrap_process();

void thread_process()
{
  //TLM-2 generic payload transaction, reused across calls to b_transport
  tlm::tlm_generic_payload* trans = new tlm::tlm_generic_payload;
  sc_time delay = sc_time(10, SC_NS);

//Generate rando sequence of reads and writes
  while(true)
  {
   readinputvalue = PIPES_ch.read();
   tlm::tlm_command cmd = static_cast<tlm::tlm_command>(rand() % 2);
   if (cmd == tlm::TLM_WRITE_COMMAND) data = 0xFF000000;
   //Initialize 8 out of 10 attributes, byte_enable_lenght and axtensions being unused
   trans->set_command( cmd );
   trans->set_address( 100 );
   trans->set_data_ptr( reinterpret_cast<unsigned char*>(&data) );
   trans->set_data_length( readinputvalue );
   trans->set_streaming_width ( 4 );
   trans->set_byte_enable_ptr(0);
   trans->set_dmi_allowed( false );
   trans->set_response_status( tlm::TLM_INCOMPLETE_RESPONSE );  //Mandatory initial value
   socket->b_transport( *trans, delay );
   if ( trans->is_response_error() )
     SC_REPORT_ERROR("TLM-2", "Response error from b_transport");
    cout << "trans = { " << (cmd ? 'W' : 'R') << ", " << hex
         << " } , data = " << hex << data << " at time " << sc_time_stamp()
         << " delay = " << delay << endl;
    wait(delay);
  }
 }
int data;
int readinputvalue;
};

#endif
