#ifndef BUSSTRUCTS_H
#define BUSSTRUCTS_H

//Basic data_t struct
struct data_t {
  sc_uint<4>  bus_signal;
  sc_uint<4>  bus_id;
  sc_uint<8>  bus_addr;
  sc_uint<16> bus_data;
};

//SCV extension class that extends data_t struct to allow randomisation
template<>
class scv_extensions<data_t> : public scv_extensions_base<data_t> {
public:
  scv_extensions<sc_uint<4> >  bus_signal;
  scv_extensions<sc_uint<4> >  bus_id;
  scv_extensions<sc_uint<8> >  bus_addr;
  scv_extensions<sc_uint<16> > bus_data;

  SCV_EXTENSIONS_CTOR(data_t) {
    //must be in order
    SCV_FIELD(bus_signal);
    SCV_FIELD(bus_id);
    SCV_FIELD(bus_addr);
    SCV_FIELD(bus_data);
  }
};


template <typename T1, typename T2>
class stimulus_class {};

template<>
class stimulus_class<data_t, uint32_t> : public data_t {
public:
    scv_smart_ptr<data_t> smptr;        //note: scv "smart_pointer" is using "scv_extensions" - these are linked somehow inside scv library
    data_t        data_struct;
    std::uint32_t data_vector;
    std::vector<sc_uint<8> > pending_read_addresses;
    bool last_read;

    stimulus_class(scv_smart_ptr<data_t> &smrtptr) : smptr(smrtptr) {
        //Set randomisation parameters
        smptr->bus_signal.disable_randomization();

        //Set initial values
        data_struct.bus_signal = 0;
        data_struct.bus_id = 0;
        data_struct.bus_addr = 0;
        data_struct.bus_data = 0;
        data_vector = 0;
        pending_read_addresses.clear();
        last_read = false;
    }

    void gen_write() {
        smptr->next();
        data_struct.bus_signal = 1;
        data_struct.bus_id = smptr->bus_id;
        data_struct.bus_addr = smptr->bus_addr;
        data_struct.bus_data = smptr->bus_data;
        //put data into pending_read_addresses list
        pending_read_addresses.push_back(data_struct.bus_addr);
        sync_vector_with_struct();
        last_read = false;
    }

    bool gen_read() {
        if (!pending_read_addresses.empty()) {
            smptr->next();
            data_struct.bus_signal = 0;
            data_struct.bus_id = smptr->bus_id;
            data_struct.bus_addr = pending_read_addresses.back();
            data_struct.bus_data = smptr->bus_data;
            //pop data from pending_read_addresses list
            pending_read_addresses.pop_back();
            sync_vector_with_struct();
            //if after pop no more pending reads exist then set last_read flag to true
            if (pending_read_addresses.empty()) {
                last_read = true;
            } else {
                last_read = false;
            }
            return true;
        } else {
            return false;
        }
    }

    void sync_vector_with_struct() {
            sc_bv<32> bv;
            bv.range(31, 28) = data_struct.bus_signal;
            bv.range(27, 24) = data_struct.bus_id;
            bv.range(23, 16) = data_struct.bus_addr;
            bv.range(15, 0) =  data_struct.bus_data;
            data_vector = bv.to_uint();
    }
};


template <typename T1, typename T2>
class response_class {};

template<>
class response_class<data_t, uint32_t> : public data_t {
public:
    data_t        rtl_data_struct, model_data_struct;
    std::uint32_t rtl_data_vector, model_data_vector;

    response_class() {
        //Set initial values
        rtl_data_struct.bus_signal = 0;
        rtl_data_struct.bus_id = 0;
        rtl_data_struct.bus_addr = 0;
        rtl_data_struct.bus_data = 0;
        rtl_data_vector = 0;
        model_data_struct.bus_signal = 0;
        model_data_struct.bus_id = 0;
        model_data_struct.bus_addr = 0;
        model_data_struct.bus_data = 0;
        model_data_vector = 0;
    }

    void sync_structs_with_vectors() {
            sc_bv<32> bv = rtl_data_vector;
            rtl_data_struct.bus_signal = bv.range(31, 28).to_uint();
            rtl_data_struct.bus_id = bv.range(27, 24).to_uint();
            rtl_data_struct.bus_addr = bv.range(23, 16).to_uint();
            rtl_data_struct.bus_data = bv.range(15, 0).to_uint();
            bv = model_data_vector;
            model_data_struct.bus_signal = bv.range(31, 28).to_uint();
            model_data_struct.bus_id = bv.range(27, 24).to_uint();
            model_data_struct.bus_addr = bv.range(23, 16).to_uint();
            model_data_struct.bus_data = bv.range(15, 0).to_uint();
    }

    bool compare_structs() {
        bool is_equal = true;
        if (rtl_data_struct.bus_signal != model_data_struct.bus_signal) {
            cout << "bus_signal differs: \"" << rtl_data_struct.bus_signal << "\" vs \"" << model_data_struct.bus_signal << "\"" << endl;
            is_equal = false;
        };
        if (rtl_data_struct.bus_id != model_data_struct.bus_id) {
            cout << "bus_id differs: \"" << rtl_data_struct.bus_id << "\" vs \"" << model_data_struct.bus_id << "\"" << endl;
            is_equal = false;
        };
        if (rtl_data_struct.bus_addr != model_data_struct.bus_addr) {
            cout << "bus_addr differs: \"" << rtl_data_struct.bus_addr << "\" vs \"" << model_data_struct.bus_addr << "\"" << endl;
            is_equal = false;
        };
        if (rtl_data_struct.bus_data != model_data_struct.bus_data) {
            cout << "bus_data differs: \"" << rtl_data_struct.bus_data << "\" vs \"" << model_data_struct.bus_data << "\"" << endl;
            is_equal = false;
        };
        return is_equal;
    }
};

#endif
