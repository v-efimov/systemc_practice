#include "systemc.h"
#include "arbiter.h"

void ARBITER::arbiter() {
  //int valX = 777;
  //int ReadFIFO1, ReadFIFO2;
  while (true) {
    //ReadFIFO1 = inp1->read();
    //ReadFIFO2 = inp2->read();
    //std::cout << sc_time_stamp() << ": reads from fifo channel1, val=" << ReadFIFO1 << std::endl;
    //std::cout << sc_time_stamp() << ": reads from fifo channel2, val=" << ReadFIFO2 << std::endl;
    //so1.write(valX++);

    wait(inp1->data_written_event() | inp2->data_written_event());
    std::cout << sc_time_stamp() << "WRITE EVENT HAPPENED!!!" << std::endl;
    wait(SC_ZERO_TIME);
  }
}