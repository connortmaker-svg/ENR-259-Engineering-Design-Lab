#ifndef TurbineControl_h
#define TurbineControl_h

#include "Arduino.h"
#include <Servo.h>

class TurbineControl {
  public:
    TurbineControl(int pin);
    void begin();
    void arm();
    void turnOn();
    void turnOff();
    void debugControl(); // Optional: exposing internal control for debugging

  private:
    int _pin;
    Servo _esc;
};

#endif
