#include "Arduino.h"
#include "M0603B.h"
#include <SoftwareSerial.h>

M0603B::M0603B(uint8_t  Rx, uint8_t  Tx) : _serial(Rx, Tx){
    
}

