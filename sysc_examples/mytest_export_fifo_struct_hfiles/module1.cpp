#include "systemc.h"
#include "module1.h"

void MODULE1::writer() {
  int x = 1;
  int y = 2;
  while (true) {
    s.write(CUSTOMIZED_TYPE{x++, y+=2});
    wait(SC_ZERO_TIME);
    if (x >= 10) {
      sc_stop();
    }
  }
}
