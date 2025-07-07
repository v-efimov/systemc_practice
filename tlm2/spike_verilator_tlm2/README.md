## This example shows a way of making an early model comprised of RISCV Spike ISS simulator coupled with Verilated RTL extensions.

The setup is comprised of the following blocks:

1. Spike module (RISC_V ISS simulator - master)
2. Uncore module (converts from TLM2.0 formed by spike to bus bit vectors)
3. TLM_to_RTL bus adapters (translate from event based TLM queues to clocked pins with valid/ready signals)
4. Verilated RTL (pipelined memory block - slave)



### Spike to Uncore interaction:
Spike is a class. Spike_step is running in the forever loop inside SC_THREAD (spike_thread).
Spike is communicating with another module Uncore using tlm2.0 b_transport and using tlm_generic_payload.

Uncore implements b_transport callback that takes data from the tlm_generic_payload that spike provides, make transaction validity checks and forms req data bit vector which is mapped to physical valid/ready bus protocol.

Once uncore b_transport triggered by spike it makes two blocking calls write.req and read.resp the the locally instantiated req sc_fifo and resp sc_fifo queues.

It shall be noted that the originator of these blocking write.req and read.resp calls is actually spike_step method that makes b_transport call. But as was mentioned spike is running in the SC_THREAD (spike_thread) and therefore spike indirectly make calls to blocking write and read methods of sc_fifos defined in the uncore module.

This TLM2.0 approach make model engineers only work with TLM2.0 generic payload structure and to be abstracted from the pin level bus implementation.


### Uncore to RTL interaction (via TLM_to_RTL bus adapters)
There are TLM_to_RTL adapter called queue2pins and RTL_to_TLM adapter called pins2queue.
Uncore is based on event based simulation flow but it needs to interact with RTL so these converters from sc_fifo queues to pin level valid/ready hardware bus protocol with clock are required.
Queue2pins and Pins2queue are sc_methods sensitive to clock.

When uncore converts TLM2.0 transaction from spike it forms bus bit vectors and places them in the sc_fifo req queue. Then there is a queue2pins module that runs on every clock event (SC_METHOD) and moves data from that queue to latch it on the Verilator interconnect registers (sc_signals) which are directly mapped to RTL valid/data/ready bus pins. If there is a stall in the RTL the data is not being taken from the queue. The moving of the data from the queue and registering it on the registers happens if (~down_valid || down_ready). This allows back-to-back data transfers in case RTL constantly ready to receive the data.

On the other side there is pins2queue module that samples data from pins and if there is a place in the queue it puts data on that queue. It checks available remaining place after data is written and provide corresponding backpressure for the RTL (again of the verilator interconnect register). There is a one clock delay between check of the remaining place in the sc_fifo and the time when RTL will see this registered backpressure signal. To allow for back to-back transfers pins2queue converter has expansion register and acts as a skid-buffer.

**Remarks:**
*1. Currently we are initiating a single transaction ad waiting for its completion. So we don't observe back-to-back transactions. It is easy to extend the model for burst transactions support and in that case queue2pins and pins2queue support for back-to-back transactions will be needed.*
*2. The pin level bus protocol is a single valid/ready channel with 32bit data vector which are split into the following (4bits operation, 4bits ID, 8bits address, 16 bits data).*
*3. In spike_module we dont have ISS simulator yet so we just imitate spike by communicating with the RTL memory block using keyboard input- entering address, data, read/write operation in the spike_loop.*


## Transactions flow:

This diagram shows transaction flow where stages are marked with red numbers.
![Image](./media/Spike_TLM20.jpg)

1. Spike is running in the SC_THREAD it is usually spike_step running in forever loop. Spike communicates with external world via b_transport function call at the point when instruction lifecycle requires this communication.
2. When spike needs to do bus operations it forms TLM2.0 generic payload and calls b_transport.
3. Corresponding b_transport callback function is called in uncore_module.
4. This b_transport function has easy access to local sc_fifos instantiated in the same module as well as access to tlm_generic payload that spike_module constructed.
5. b_transport takes the data from tlm_generic payload and forms bitvector that will go into hardware pin level bus. It sends this vector to RTL by doing blocking write into req sc_fifo.
6. Lets assume there is space in sc_fifo at step 5 and write to req queue didn't block. In this case b_trasport function needs the bus response and do blocking read from resp sc_fifo. When b_transport in uncore_module tries to read from sc_fifo there is no data becuase RTL hasn't processed the request yet so read from sc_fifo response queue blocks. Because it is being called from spike_thread that blocking read will block it. So the spike_thread will wait till write_event into sc_fifo resp queue happens.
7. So from now on spike_thread gave control to the systemc simulator that processes other events. So it just processes clock events and let RTL run.
8. Clock events trigger all processes sensitive to clock. In particular it will cause data being taken from req sc_fifo.
9. Then while spike_thread is still waiting for data being written in sc_fifo resp queue RTL keeps running.
10. When valid data is at the end of RTL pipeline it get written by pins2queue transactor into sc_fifo resp queue.
11. At this point spike thread can resume execution and continue with reading from sc_fifo resp queue.
12. namely b_transport function can now get data from resp sc_fifo in the form bit vector.
13. b_transport in uncore converts this vector updating tlm_generic_payload fields. Then b_transport callback function returns control to spike b_transport function.
14. Spike b_transport function finish execution so spike now have updated tlm_generic_payload with the output from the RTL and can continue processing its current instruction of go to another instruction.

*Note: For spike the call to b_transport looks like a single function call, however during this call systemc simulation time will advance and needed number of clock cycles will be processed for RTL to produce the response for the request.*


