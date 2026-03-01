#define MOTORID 0x01
#define BAUD_RATE 115200


int motorID; //1 for L 2 for R
int motorSpeed = 200;
int delay = 5000;

//TO DO:
// make velocity library

void setup() {
  Serial.begin(115200); // Debug Monitor
  Serial1.begin(BAUD_RATE); // Motor Port Left
  Serial2.begin(BAUD_RATE); // Motor Port Right
  motorId = 1;
  setMotorSpeed(50);
  motorId = 2;
  // program under here:

  while(delay > 0){
    setMotorSpeed(motorSpeed);
    motorSpeed = motorSpeed - 5;
    delay(delay);
    delay = delay-50;
  }
}

void loop() {
  driveBreak(); //break both wheels
  while(1);
}



void setPositionMode() {

  uint8_t packet[10] = {
    MOTORID,
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

void setVelocityMode() {

  uint8_t packet[10] = {
    MOTORID,
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

// send speed command
void setMotorSpeed(int16_t rpm) {
  int16_t speedValue = rpm * 10;
  uint8_t packet[10] = {
    MOTORID,
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
    MOTORID,
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

void requestMotorMode() {

  uint8_t packet[10] = {
    MOTORID,
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

void checkID(){
  Serial.print("Current ID: ");
  Serial.println(motorID);
}


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

void setBothTiresToVelocityMode(){
  int oldID = motorID;
  driveBreak();
  motorID = 1;
  setVelocityMode();
  motorID = 2;
  setVelocityMode();
  motorID = oldID;
}


