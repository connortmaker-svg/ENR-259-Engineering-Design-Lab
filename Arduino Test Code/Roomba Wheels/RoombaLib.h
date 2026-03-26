/* This file contain the various functions used to communicate with and control
   the servo wheels. It uses UART communication with 10 byte data packets and a CRC
   algorithm to handle errors in the data.
*/


//Sets the wheel to positional mode
void setPositionMode() {

  uint8_t packet[10] = {
    0x01,
    0xA0,
    0x03,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00
  };

  calculateAndSend(packet);
}
//Set the wheel to velocity mode
void setVelocityMode() {

  uint8_t packet[10] = {
    0x01,
    0xA0,
    0x02,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00
  };

  calculateAndSend(packet);
}

//send speed command
void setMotorSpeed(int16_t rpm) {
  int16_t speedValue = rpm * 10;
  uint8_t packet[10] = {
    0x01,
    0x64,
    (speedValue >> 8) & 0xFF,
    speedValue & 0xFF,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00
  };
  calculateAndSend(packet);
}

void requestMotorStatus() {

  uint8_t packet[10] = {
    0x01,
    0x74,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00
  };

  calculateAndSend(packet);
}

//Sets the wheel ID for one wheel
void setID(uint8_t id) {

  uint8_t packet[10] = {
    0xAA,
    0x55,
    0x53,
    id,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00
  };

  if (id == 0x01) {
    for (int i = 0; i < 6; i++) {
      Serial1.write(packet, 10);

    }
  } else {
    for (int i = 0; i < 6; i++) {
      Serial2.write(packet, 10);
    }
  }

}

//Requests the current operating mode of the motor (0x2 = velocity mode, 0x3 = position mode)
void requestMotorMode() {

  uint8_t packet[10] = {
    0x01,
    0x75,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00
  };

  calculateAndSend(packet);
}

// calc crc and send to motor
void calculateAndSend(uint8_t * packet) {
  // crc8/maxim evil evil evil evile
  //copy pasted from stack overflow. credit lost to control c control v
  uint8_t crc = 0x00;
  for (int i = 0; i < 9; i++) {
    crc ^= packet[i];
    for (int j = 0; j < 8; j++) {
      if (crc & 0x01) {
        crc = (crc >> 1) ^ 0x8C;
      } else {
        crc >>= 1;
      }
    }
  }
  packet[9] = crc;
  // Send
  Serial.println();

  if (motorID == 1) {
    Serial1.write(packet, 10);
  } else {
    Serial2.write(packet, 10);
  }
}
//Requests the ID for the current wheel
void checkID(){
  Serial.print("Current ID: ");
  Serial.println(motorID);
}

//Sends data packet that will casue the roomba to stop
void driveBreak(){
uint8_t packet[10] = {
    0x01,
    0x64,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x50
  };
  Serial1.write(packet, 10);    
  Serial2.write(packet, 10);
}

//Sets both wheels to operate in velocity mode
void setBothTiresToVelocityMode(){
  int oldID = motorID;
  driveBreak();
  motorID = 1;
  setVelocityMode();
  motorID = 2;
  setVelocityMode();
  motorID = oldID;
}