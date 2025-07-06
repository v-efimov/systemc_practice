#ifndef PINS2QUEUE_H
#define PINS2QUEUE_H

template <typename T>
struct pins2queue_module: sc_module {
  sc_in_clk clk_port;
  sc_port<sc_signal_in_if<bool>> rstn_port;

  sc_port<sc_signal_in_if<bool>> up_valid;
  sc_port<sc_signal_in_if<T>> up_data;
  sc_port<sc_signal_out_if<bool>> up_ready;
  sc_port<sc_fifo_out_if<T>> down_PIPE;

  sc_signal<T> REG;
  sc_signal<bool> state;
  enum BufState { BYPASS=false, SKID=true };

  SC_CTOR(pins2queue_module) {
    SC_METHOD(pins2queue_method);
    sensitive << clk_port.pos();
  }
  ~pins2queue_module() {
    std::cout << sc_time_stamp() << ": Cleanup: destructor" << std::endl;
  }
  void pins2queue_method();
};

#endif