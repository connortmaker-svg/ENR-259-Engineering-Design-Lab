#define MOTORID 0x01
#define BAUD_RATE 115200


int motorID; //1 for L 2 for R

//TO DO:
// go for certain speed
// make library? proper?

void setup() {
  Serial.begin(115200); // Debug Monitor
  Serial1.begin(BAUD_RATE); // Motor Port Left
  Serial2.begin(BAUD_RATE); // Motor Port Right

  delay(1000); // Wait before next loop
  debugPrintout();
}

void loop() {
  if (Serial.available() > 0) {
    int sel = Serial.parseInt();
    //clear buffer
    while (Serial.available()) {
      Serial.read();
    }
    debugMenu(sel);
    debugPrintout();
  }
}

void debugPrintout() {
  Serial.println("  \n  -------- MENU --------");
  Serial.println("1.         Set Current Motor");
  Serial.println("2.           Set Motor Speed");
  Serial.println("3.           Set Motor Angle");
  Serial.println("4.                Debug Test");
  //Serial.println("5. Get Motor Mode 0x02 Right");
  Serial.println("6.         Set Velocity Mode"); //not necessary
  Serial.println("7.           Set Degree Mode"); //not necessary
  Serial.println("8.          Break Both Tires");
  Serial.println("9.       Turn x degrees Left");
  Serial.println("10.     Turn x degrees Right");
  Serial.println("11.      Check current Motor");
  Serial.println("   Input Choice: ");
}

void debugMenu(int selection) {
  delay(200);
  switch (selection) {
  case 1:
    case1();
    
    break;
  case 2:
    Serial.println("NOTE. IF YOU WANT TO SWITCH FROM VELOCITY TO ANGLE,");
    Serial.println("YOU MUST FIRST MANUALLY SET VELOCITY TO 0");
    case2();
    break;
  case 3:
    Serial.println("NOTE. IF YOU WANT TO SWITCH FROM VELOCITY TO ANGLE,");
    Serial.println("YOU MUST FIRST MANUALLY SET VELOCITY TO 0");
    case3();
    break;
  case 4:
    setBothTiresOn();
    break;
  case 5:
    
    break;
  case 6:
    setVelocityMode();
    break;
  case 7:
    setPositionMode();
    break;
  case 8:
    driveBreak();
    break;
  case 9:
    case9();
    break;
  case 10:
    case10();
    break;
  case 11:
     Serial.print("Current Motor: ");
     if(motorID == 1){
      Serial.println("Left");
     } else{
      Serial.println("right");
     }
    break;
  default:
    Serial.println("Invalid choice.");
    break;
  }
}

void case1() {
  Serial.println("Left Motor: 0x01 ");
  Serial.println("Right Motor: 0x02 ");
  Serial.print("Input Here: ");

  // Read id
  
  waitForUserInput();
  motorID = Serial.parseInt();

  Serial.print("New Motor ID: ");
  Serial.println(motorID);
  
   
}

void case2() {
  clearAllBuffers();
  Serial.print("Enter Speed (-210 to 210): ");

  setVelocityMode(); //set to correct motor mode
  waitForUserInput();

  // Read speed
  int speed = Serial.parseInt();

  //clear buffer
  clearBufferSerial();

  Serial.print("Setting speed to: ");
  Serial.println(speed);
  delay(10);
  setMotorSpeed(speed);
}

void case3() {
  setMotorSpeed(0);
  Serial.println("(0-360) but maps like -180 to 180");
  Serial.print("Enter angle (0 to 360): ");

  waitForUserInput();

  // Read speed
  int deg = Serial.parseInt();

  //clear buffer
  clearBufferSerial();

  Serial.print("Setting angle to: ");
  Serial.println(deg);
  delay(10);
  setPositionMode(); //set to angle mode
  setMotorDegrees(deg);
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

void clearBufferSerial() {
  while (Serial.available()) {
    Serial.read();
  }
}

void clearBufferSerial1() {
  while (Serial1.available()) {
    Serial1.read();
  }
}

void clearBufferSerial2() {
  while (Serial2.available()) {
    Serial2.read();
  }
}

void clearAllBuffers() {
  clearBufferSerial();
  clearBufferSerial1();
  clearBufferSerial2();
}

void setMotorDegrees(int16_t deg) {

  float degVal_float = (float) 32767 / (float) 360 * (float) deg;
  uint16_t degVal_int = (uint16_t) ceil(degVal_float);

  Serial.print("DegVal_int: ");
  Serial.println(degVal_int);

  uint8_t packet[10] = {
    MOTORID,
    0x64,
    (degVal_int >> 8) & 0xFF,
    degVal_int & 0xFF,
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

void waitForUserInput() {
  while (Serial.available() <= 0) {}
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

void turnLeft(int deg){
  driveBreak();
  motorID = 2;
  setMotorDegrees(deg);
}

void turnRight(int deg){
  driveBreak();
  motorID = 1;
  setMotorDegrees(deg);
}

void case9() {
  clearAllBuffers();
  setBothTiresToPosMode();
  Serial.println("(0-360) but maps like -180 to 180");
  Serial.print("Enter angle (0 to 360): ");

  setPositionMode(); //set to angle mode
  waitForUserInput();

  // Read speed
  int deg = Serial.parseInt();

  //clear buffer
  clearBufferSerial();

  turnLeft(deg);
}


void case10() {
  clearAllBuffers();
  setBothTiresToPosMode();
  Serial.println("(0-360) but maps like -180 to 180");
  Serial.print("Enter angle (0 to 360): ");

  setPositionMode(); //set to angle mode
  waitForUserInput();

  // Read speed
  int deg = Serial.parseInt();

  //clear buffer
  clearBufferSerial();

  turnRight(deg);
}

void setBothTiresToPosMode(){

  driveBreak();
  motorID = 1;
  setPositionMode();
  motorID = 2;
  setPositionMode();
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

void setBothTiresOn(){
  setBothTiresToVelocityMode();
  motorID = 1;
  setMotorSpeed(100);
  motorID = 2;
  setMotorSpeed(100);
  
  delay(2000);
  
  motorID = 1;
  setMotorSpeed(0);
  motorID = 2;
  setMotorSpeed(0);
}
