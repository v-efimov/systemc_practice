#include "systemc.h"
#include "testbench.h"

void testbench_module::tbgen_thread() {
  sc_time delay = sc_time(100, SC_NS);
  int do_transfers;

  for (do_transfers = 0; do_transfers < 100; do_transfers++) {
    stimulus.gen_write();
    req_PIPE_ch.write(stimulus.data_vector);
    ref_PIPE_ch.write(stimulus.data_vector);
    trtocken_PIPE_ch.write(1);
    wait(no_delay);
  }

  for (do_transfers = 0; do_transfers < 100; do_transfers++) {
    stimulus.gen_read();
    req_PIPE_ch.write(stimulus.data_vector);
    ref_PIPE_ch.write(stimulus.data_vector);
    if (do_transfers == 99) {
        trtocken_PIPE_ch.write(0);
    } else {
        trtocken_PIPE_ch.write(1);
    }
    wait(long_delay);
  }

}
