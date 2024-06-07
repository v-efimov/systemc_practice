#include "systemc.h"
#include "arbiter.h"

void ARBITER::arbiter() {
  int ReadFIFO1, ReadFIFO2;
  int NumFIFO1, NumFIFO2;
  int rrtoggle = 0;
  int carryon;
  while (true) {
    wait(inp1->data_written_event() | inp2->data_written_event());
    NumFIFO1 = inp1->num_available();
    NumFIFO2 = inp2->num_available();
    std::cout << sc_time_stamp() << "WRITE EVENT HAPPENED!!!, FIFO1/FIFO2 BEFORE READ:" << NumFIFO1 << "/" << NumFIFO2 << std::endl;
    carryon = 1;
    while (carryon == 1) {
        if (rrtoggle == 0) {
            if (inp1->nb_read(ReadFIFO1) == true) {
                outp1->write(ReadFIFO1);
                //std::cout << sc_time_stamp() << "ReadFIFO1 data: " << ReadFIFO1 << "---" << std::endl;
                rrtoggle = 1;
            }
        }
        else
        {
            if (inp2->nb_read(ReadFIFO2) == true) {
                outp1->write(ReadFIFO2);
                //std::cout << sc_time_stamp() << "ReadFIFO2 data: " << ReadFIFO2 << "---" << std::endl;
                rrtoggle = 0;
            }
        }
        if (inp1->num_available() == 0 && inp1->num_available() == 0) {
            std::cout << "STOP FETCHING" << std::endl;
            carryon = 0;
        }
    }
    wait(SC_ZERO_TIME);
  }
}
