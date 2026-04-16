// BricoGeek_DirectDriveServo.h

#ifndef BricoGeek_DirectDriveServo_h
#define BricoGeek_DirectDriveServo_h

#include "Arduino.h"

class BricoGeek_DirectDriveServo {
public:
    // Constructor now takes a reference to a HardwareSerial object (e.g., Serial1)
    BricoGeek_DirectDriveServo(HardwareSerial& serial, uint8_t id = 0x01);

    void begin(long baudRate = 115200);
    void setMode(uint8_t mode);
    void setSpeed(int16_t speed);
    void setPosition(uint16_t position);
    void brake();
    void setID(uint8_t newID);

    uint8_t getID();
    uint8_t getMode();
    int16_t getSpeed();
    uint16_t getPosition();
    uint8_t getTemperature();
    uint8_t getError();

private:
    uint8_t _id;
    HardwareSerial& _serial; // Reference to the HardwareSerial port

    void sendCommand(uint8_t command, int16_t data = 0);
    uint8_t calculateChecksum(uint8_t* packet, uint8_t length);
    void sendPacket(uint8_t* packet, uint8_t length);
    void readResponse(uint8_t* buffer, uint8_t length);
};

#endif
