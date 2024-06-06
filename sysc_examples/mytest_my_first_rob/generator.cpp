#include "systemc.h"
#include "generator.h"

void GENERATOR::generator() {
  int val1 = 1;
  int val2 = 1000;
  while (true) {
    s1.write(val1++);
    s2.write(val2++);
    wait(SC_ZERO_TIME);
    if (val1 >= 10) {
      sc_stop();
    }
  }
}
