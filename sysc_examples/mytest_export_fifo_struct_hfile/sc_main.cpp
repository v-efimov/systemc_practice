// SystemC global header
#include <systemc.h>
#include <ostream>

using namespace sc_core;

struct CUSTOMIZED_TYPE {
  int x, y;
  CUSTOMIZED_TYPE(int x = 0, int y = 0) : x(x), y(y) {}
  CUSTOMIZED_TYPE& operator=(const CUSTOMIZED_TYPE& rhs) {
    x = rhs.x;
    y = rhs.y;
    return *this;
  }

  bool operator==(const CUSTOMIZED_TYPE& rhs) {
    return x == rhs.x && y == rhs.y;
  }

  friend std::ostream& operator << (std::ostream& os, const CUSTOMIZED_TYPE& val) {
    os << "x = " << val.x << "; y = " << val.y << std::endl;
    return os;
  }

};


//inline void sc_trace(sc_trace_file*& f, const CUSTOMIZED_TYPE& val, std::string name) {
//  sc_trace(f, val.x, name + ".x");
//  sc_trace(f, val.y, name + ".y");
//}


SC_MODULE (MODULE1) {
  sc_export<sc_fifo<CUSTOMIZED_TYPE>> p;
  sc_fifo<CUSTOMIZED_TYPE> s;

  SC_CTOR(MODULE1) {
    p(s);
    SC_THREAD(writer);
  }
  ~MODULE1() {
    std::cout << sc_time_stamp() << ": Cleanup: destructor" << std::endl;
  }

void writer() {
  int x = 1;
  int y = 1;
  while (true) {
    s.write(CUSTOMIZED_TYPE{x++, y+=2});
    wait(SC_ZERO_TIME);
    if (x >= 10) {
      sc_stop();
    }
  }
 }
};


SC_MODULE(MODULE2) {
  sc_port<sc_fifo_in_if<CUSTOMIZED_TYPE>> p;
  SC_CTOR(MODULE2) {
    SC_THREAD(reader);
  }
  ~MODULE2() {
    std::cout << sc_time_stamp() << ": Cleanup: destructor" << std::endl;
  }

void reader() {
  while (true) {
    std::cout << sc_time_stamp() << ": reads from fifo channel: " << p->read() << std::endl;
  }
 }
};


int sc_main(int, char* []) {
    MODULE1 module1("module1");
    MODULE2 module2("module2");
    module2.p(module1.p);

    sc_start(10, SC_SEC);
    return 0;
}
