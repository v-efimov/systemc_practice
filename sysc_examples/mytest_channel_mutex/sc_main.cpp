// SystemC global header
#include <systemc.h>
#include <iostream>

using namespace sc_core;


SC_MODULE (MUTEX) {
  sc_mutex m;
  SC_CTOR(MUTEX) {
    SC_THREAD(thread_1);
    SC_THREAD(thread_2);
  }


void thread_1() {
  while (true) {
    if (m.trylock() == -1) {
      m.lock();
      std::cout << sc_time_stamp() << ": thread_1 obtained resource by lock()" << std::endl;
      cin.get();
                           }
    else {
      std::cout << sc_time_stamp() << ": thread_1 obtained resource by trylock()" << std::endl;
      cin.get();
         }
    wait(1, SC_SEC);
    m.unlock();
    std::cout << sc_time_stamp() << ": unlocked by thread_1" << std::endl;
    wait(SC_ZERO_TIME);
   }
 }


void thread_2 () {
   while (true) {
     if (m.trylock() == -1) {
       m.lock();
       std::cout << sc_time_stamp() << ": thread_2 obtained resource by lock()" << std::endl;
       cin.get();
                            }
     else {
       std::cout << sc_time_stamp() << ": thread_2 obtained resource by trylock()" << std::endl;
       cin.get();
          }
     wait(1, SC_SEC);
     m.unlock();
     std::cout << sc_time_stamp() << ": unlocked by thread_2" << std:: endl;
     wait(SC_ZERO_TIME);
    }
  }
};


int sc_main(int, char* []) {
    MUTEX mutex("mutex");
    sc_start(4, SC_SEC);
    return 0;
}
