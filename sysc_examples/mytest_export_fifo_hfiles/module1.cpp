#include "systemc.h"
#include "module1.h"

void MODULE1::writer() {
  int val = 1;
  while (true) {
    s.write(val++);
    wait(SC_ZERO_TIME);
    if (val >= 10) {
      sc_stop();
    }
  }
}
