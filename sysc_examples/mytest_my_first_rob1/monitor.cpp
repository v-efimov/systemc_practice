#include "systemc.h"
#include "monitor.h"

void MONITOR::monitor() {
  int ReadMON;
  while (true) {
    ReadMON = so1.read();
    std::cout << sc_time_stamp() << ": MONITOR reads, val=" << ReadMON << std::endl;
  }
}