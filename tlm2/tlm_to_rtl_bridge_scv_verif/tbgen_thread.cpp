#include "systemc.h"
#include "testbench.h"

void testbench_module::tbgen_thread() {

  const int NUM_OF_MODES = 3;                                    //we have three modes FAST, RANDOM, SLOW for each component
  const int MAX_MODES_NUM = (NUM_OF_MODES*NUM_OF_MODES) - 1;     //for now we have run all combinations of these modes for at least GEN and CHECK components (maybe MODEL in the future).
  const int NUM_OF_WRITES = 100;

  int tr_tocken_val;

    for (int cur_mode_index = 0; cur_mode_index <= MAX_MODES_NUM; cur_mode_index++) {
      int gen_mode = cur_mode_index / NUM_OF_MODES;
      int chk_mode = cur_mode_index % NUM_OF_MODES;

      int numwrites = 0;
      bool cur_mode_last_read = false;
      while (!cur_mode_last_read) {
          if (numwrites < NUM_OF_WRITES) {
              stimulus.gen_write();
              numwrites++;
          } else {
              stimulus.gen_read();
              cur_mode_last_read = stimulus.last_read;
          };

          if (cur_mode_index == MAX_MODES_NUM && cur_mode_last_read) {
              tr_tocken_val = FINAL_TRANSFER_TOCKEN;
          } else {
              tr_tocken_val = chk_mode;
          };

          req_PIPE_ch.write(stimulus.data_vector);
          ref_PIPE_ch.write(stimulus.data_vector);
          trtocken_PIPE_ch.write(tr_tocken_val);

          get_thread_delay(gen_delay, gen_mode);
          wait(gen_delay);
      };
  };
}
