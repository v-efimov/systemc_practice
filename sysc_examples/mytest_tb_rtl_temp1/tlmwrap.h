#ifndef TLMWRAP_H
#define TLMWRAP_H

#include "initiator.h"
#include "target.h"

SC_MODULE (TLMWRAP) {
  sc_export<sc_fifo<int>> PIPES_p;
  sc_fifo<int> PIPES_ch;
  sc_export<sc_fifo<int>> PIPEM_p;
  sc_fifo<int> PIPEM_ch;

  Initiator *initiator;
  Memory    *memory;

  SC_CTOR(TLMWRAP) : PIPES_ch(4), PIPEM_ch(4) {
    PIPES_p(PIPES_ch);
    PIPEM_p(PIPEM_ch);
    //SC_THREAD(tlmwrap_process);
    // Instantiate components
    initiator = new Initiator("initiator");
    memory    = new Memory   ("memory");
    //Bind initiator socket to target socket
    initiator->socket.bind(memory->socket);

  }
  ~TLMWRAP() {
    std::cout << sc_time_stamp() << ": Cleanup: destructor" << std::endl;
  }
  //void tlmwrap_process();
};

#endif
