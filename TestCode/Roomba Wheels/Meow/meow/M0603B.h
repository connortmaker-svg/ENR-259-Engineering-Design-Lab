#ifndef M0603B_h
#define M0603B_h

#include "Arduino.h"
#include <SoftwareSerial.h>


class M0603B {
public:
    M0603B(uint8_t  Rx, uint8_t  Tx);
    void MoveDeg(uint8_t  deg);
private:
    uint8_t  _Rx;
    uint8_t  _Tx;
    SoftwareSerial _serial;
};


#endif