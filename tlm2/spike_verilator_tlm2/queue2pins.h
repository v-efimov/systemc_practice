#ifndef QUEUE2PINS_H
#define QUEUE2PINS_H

template <typename T>
struct queue2pins_module: sc_module {
  sc_in_clk clk_port {""};
  sc_port<sc_signal_in_if<bool>> rstn_port;
  sc_port<sc_signal_out_if<bool>> down_valid;
  sc_port<sc_signal_out_if<T>> down_data;
  sc_port<sc_signal_in_if<bool>> down_ready;
  sc_port<sc_fifo_in_if<T>> up_PIPE;

  T ReadValueFromUpPIPE;

  SC_CTOR(queue2pins_module) {
    SC_METHOD(queue2pins_method);
    sensitive << clk_port.pos();
  }
  ~queue2pins_module() {
    std::cout << sc_time_stamp() << ": Cleanup: destructor" << std::endl;
  }
  void queue2pins_method();
};

#endif