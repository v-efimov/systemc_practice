#include "systemc.h"
#include "generator.h"

void GENERATOR::generator() {
  int val1 = 1;
  int val2 = 1000;
  while (true) {
    s1.write(val1++);
    std::cout << sc_time_stamp() << "WRITE PORT1" << std::endl;
    s2.write(val2++);
    std::cout << sc_time_stamp() << "WRITE PORT2" << std::endl;
    wait(SC_ZERO_TIME);
    if (val1 >= 10) {
      sc_stop();
    }
  }
}
