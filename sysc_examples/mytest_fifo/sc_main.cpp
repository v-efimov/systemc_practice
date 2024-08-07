// SystemC global header
#include <systemc.h>

using namespace sc_core;


SC_MODULE (FIFO) {
  sc_fifo<int> f1, f2, f3;
  SC_CTOR(FIFO) : f1(2), f2(2), f3(2) {
    SC_THREAD(generator1);
    SC_THREAD(consumer1);
    SC_THREAD(generator2);
    SC_THREAD(consumer2);
    SC_THREAD(generator3);
    SC_THREAD(consumer3);
  }


void generator1() {
  int v = 0;
  while (true) {
    f1.write(v);
    std::cout << sc_time_stamp() << ": generator1 writes " << v++ << std::endl;
    wait(1, SC_SEC);
   }
 }


void consumer1() {
  int v = -1;
  while (true) {
    f1.read(v);
       std::cout << sc_time_stamp() << ": consumer1 reads " << v << std::endl;
       wait(3, SC_SEC);
  }
}

void generator2() {
  int v = 0;
  while (true) {
    while (f2.nb_write(v) == false) {
      wait(f2.data_read_event());
  }
  std::cout << sc_time_stamp() << ": generator2 writes " << v++ << std::endl;
  wait(1, SC_SEC);
  }
}

void consumer2() {
  int v = -1;
  while (true) {
    while (f2.nb_read(v) == false) {
      wait(f2.data_written_event());
   }
   std::cout << sc_time_stamp() << ": consumer2 reads " << v << std::endl;
   wait(3, SC_SEC);
   }
}

void generator3() {
  int v = 0;
  while (true) {
    std::cout << sc_time_stamp() << ": generator3, before write, #free/#available=" << f3.num_free() << "/" << f3.num_available() << std::endl;
    f3.write(v++);
    std::cout << sc_time_stamp() << ": generator3, after write, #free/#available=" << f3.num_free() << "/" << f3.num_available() << std::endl;
    wait(1, SC_SEC);
    }
}

void consumer3() {
  int v = -1;
  while (true) {
    std::cout << sc_time_stamp() << ": consumer3, before read, #free/#available=" << f3.num_free() << "/" << f3.num_available() << std::endl;
    f3.read(v);
    std::cout << sc_time_stamp() << ": consumer3, after read, #free/#availalble=" << f3.num_free() << "/" << f3.num_available() << std::endl;
    wait(3, SC_SEC);
    }
  }

};


int sc_main(int, char* []) {
    FIFO fifo("fifo");
    sc_start(10, SC_SEC);
    return 0;
}
