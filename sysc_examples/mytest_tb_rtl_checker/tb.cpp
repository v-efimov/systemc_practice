#include "systemc.h"
#include "tb.h"

void TB::writer() {
  int data = 1;
  while (true) {
    data_ch.write(data++);
    wait();
  }
}
