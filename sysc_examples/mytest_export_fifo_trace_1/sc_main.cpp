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

  inline friend void sc_trace(sc_trace_file*& f, const CUSTOMIZED_TYPE& val, std::string name) {
    sc_trace(f, val.x, name + ".x");
    sc_trace(f, val.y, name + ".y");
  }

  friend std::ostream& operator << (std::ostream& os, const CUSTOMIZED_TYPE& val) {
    os << "x = " << val.x << "; y = " << val.y << std::endl;
    return os;
  }

};



SC_MODULE (MODULE1) {
  sc_signal<CUSTOMIZED_TYPE> qqq;
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
    qqq.write(CUSTOMIZED_TYPE{x++, y+=2});
    wait(1, SC_SEC);
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
    wait(1, SC_SEC);
  }
 }
};


int sc_main(int, char* []) {
    MODULE1 module1("module1");
    MODULE2 module2("module2");
    module2.p(module1.p);

    sc_trace_file* file = sc_create_vcd_trace_file("trace");
    sc_trace(file, module1.qqq, "customized_type");
    sc_start(10, SC_SEC);
    sc_close_vcd_trace_file(file);
    return 0;
}
