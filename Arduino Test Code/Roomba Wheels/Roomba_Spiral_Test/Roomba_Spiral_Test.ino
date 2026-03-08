
#define BAUD_RATE 115200

int motorID; //1 for L 2 for R
const int BASE_SPEED = 100; // spd increases scale of path
const int START_TURN = 20; // Initial turn magnitude
const int TURN_DECAY = 1; // Increment rate (how fast it tightens)
const int SPIRAL_STEPS = 100; // steps //increase: increases path length //decrease: decreases path length (how long the robot moves on the spiral)
const int STEP_DELAY = 100; // delay //increase: increases length of linear movement. lower: less jank, higher: more jank ((resolution))

void setup() {

  Serial.begin(BAUD_RATE); // Debug Monitor
  Serial1.begin(BAUD_RATE); // Motor Port Left
  Serial2.begin(BAUD_RATE); // Motor Port Right
  Serial.println("Start Spiral");

 

  for (int i = 0; i < SPIRAL_STEPS; i++) {

    int currentTurn = (i * TURN_DECAY) + START_TURN; 

    int speedLeft = BASE_SPEED - currentTurn;  // Decrease/Reverse Left motor
    int speedRight = BASE_SPEED + currentTurn; // Increase Right motor

    if (speedLeft < -255) speedLeft = -255;
    if (speedRight > 255) speedRight = 255; 

 
    motorID = 1;
    setMotorSpeed(1 * speedLeft);
    delay(50); // delay for ms bc serial is laggy
    motorID = 2;
    setMotorSpeed(-1 * speedRight);

    Serial.print("Step: "); Serial.print(i);
    Serial.print(" | L: "); Serial.print(speedLeft);
    Serial.print(" | R: "); Serial.print(speedRight);
    Serial.print(" | Turn: "); Serial.println(currentTurn);

    delay(STEP_DELAY);
  }  
}

void loop() {
  // Stop motors after spiral is done
  Serial.println("Done!");
  motorID = 1;
  setMotorSpeed(0);
  delay(50);
  motorID = 2;
  setMotorSpeed(0);
  while(1); // Halt execution
}


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

// send speed command
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
