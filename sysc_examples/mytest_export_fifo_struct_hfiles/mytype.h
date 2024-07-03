#ifndef MYTYPE_H
#define MYTYPE_H

#include "systemc.h"
#include <ostream>
#include <iomanip>

struct CUSTOMIZED_TYPE {
    //declaration
    int x,y;

    //constructor
    CUSTOMIZED_TYPE(int x=0, int y=0) : x(x), y(y) {}

    //Can delete
    //bool operator==(const CUSTOMIZED_TYPE& rhs) {
    //  return x == rhs.x && y == rhs.y;
    //}

    //Can delete
    //CUSTOMIZED_TYPE& operator=(const CUSTOMIZED_TYPE& rhs) {
    //  x = rhs.x;
    //  y = rhs.y;
    //  return *this;
    //}

    inline friend void sc_trace(sc_trace_file*& f, const CUSTOMIZED_TYPE& val, std::string name) {
      sc_trace(f, val.x, name + ".x");
      sc_trace(f, val.y, name + ".y");
    }

    friend std::ostream& operator<<(std::ostream& os, const CUSTOMIZED_TYPE& val) {
      os << "x= " << val.x << "; y = " << val.y << std::endl;
      return os;
    }

};
#endif
