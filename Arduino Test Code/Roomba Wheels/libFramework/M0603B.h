#ifndef M0603B_h
#define M0603B_h

#include "Arduino.h"

class M0603B {
    public:
        M0603B(int Rx);
        void MoveDeg(int deg);
    private:
        int _Rx;
};

#endif