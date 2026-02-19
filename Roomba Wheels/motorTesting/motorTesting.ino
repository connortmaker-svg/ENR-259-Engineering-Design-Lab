#define MOTOR_ID_LEFT 0x01
#define MOTOR_ID_RIGHT 0x02

#define BAUD_RATE 115200

int motorID;

// to do:
// 7.) turning function turn left turn right by x degrees etc.
// 8.) make debug menu look nice cause rn it looks really ugly 

void setup() {
  Serial.begin(115200); // Debug Monitor
  Serial1.begin(BAUD_RATE); // Motor Port Left
  Serial2.begin(BAUD_RATE); // Motor Port Right

  delay(100); // Wait before next loop
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
  Serial.println("  \n-------- MENU --------");
  Serial.println("1.         Set Current Motor");
  Serial.println("2.           Set Motor Speed");
  Serial.println("3.           Set Motor Angle");
  Serial.println("4.  Get Motor Mode 0x01 Left");
  Serial.println("5. Get Motor Mode 0x02 Right");
  Serial.println("6.         Set Velocity Mode");
  Serial.println("7.           Set Degree Mode");
  Serial.println("8.                    Set ID");
  //Serial.println("9.       Turn x degrees Left");
  //Serial.println("10.     Turn x degrees Right");
  Serial.println("11.                 check ID");
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

    uint8_t query_cmd_L[10] = {
      0x01,
      0x75,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0
    };
    calculateAndSend(query_cmd_L);

    //  10-byte response: ID, 0x75,Mode value,[thing of zeros], CRC8

    while (!Serial1) yield();
    delay(2000);
    if (Serial1.available() >= 10) {
      uint8_t rx_buffer[10];
      Serial1.readBytes(rx_buffer, 10);

      // make sure that the header is 0x75 which is the response mode
      if (rx_buffer[1] == 0x75) {
        uint8_t current_mode = rx_buffer[2]; // mode val in dat[2]

        // wait for response
        switch (current_mode) {
        case 0x00:
          Serial.println("Open Loop");
          current_mode = -1;
          break;
        case 0x02:
          Serial.println("Speed Loop");
          current_mode = -1;
          break;
        case 0x03:
          Serial.println("Position Loop");
          current_mode = -1;
          break;
        default:
          Serial.println("Unknown/Error");
          break;
        }

      }
    }

    break;
  case 5:
    uint8_t query_cmd_R[10] = {
      0x02,
      0x75,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0
    };
    calculateAndSend(query_cmd_R);

    //  10-byte response: ID, 0x75,Mode value,[thing of zeros], CRC8

    while (!Serial2) yield();
    delay(2000);
    if (Serial2.available() >= 10) {
      uint8_t rx_buffer[10];
      Serial2.readBytes(rx_buffer, 10);

      // make sure that the header is 0x75 which is the response mode
      if (rx_buffer[1] == 0x75) {
        uint8_t current_mode = rx_buffer[2]; // mode val in dat[2]

        // wait for response
        switch (current_mode) {
        case 0x00:
          Serial.println("Open Loop");
          current_mode = -1;
          break;
        case 0x02:
          Serial.println("Speed Loop");
          current_mode = -1;
          break;
        case 0x03:
          Serial.println("Position Loop");
          current_mode = -1;
          break;
        default:
          Serial.println("Unknown/Error");
          break;
        }

      }
    }

    break;
  case 6:
    setVelocityMode();
    break;
  case 7:
    setPositionMode();
    break;
  case 8:
    break;
  case 9:
    break;
  case 10:
    break;
  case 11:
     Serial.print("Current ID: ");
     Serial.println(motorID);
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
  setID(motorID);

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
  clearAllBuffers();
  Serial.println("(0-360) but maps like -180 to 180");
  Serial.print("Enter angle (0 to 360): ");

  setPositionMode(); //set to angle mode
  waitForUserInput();

  // Read speed
  int deg = Serial.parseInt();

  //clear buffer
  clearBufferSerial();

  Serial.print("Setting angle to: ");
  Serial.println(deg);
  delay(10);
  setMotorDegrees(deg);
}

void setPositionMode() {

  uint8_t packet[10] = {
    motorID,
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
    motorID,
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
    motorID,
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
    motorID,
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
    motorID,
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
    motorID,
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

void velocityStop() {
  setVelocityMode();
  clearBufferSerial();
  delay(10);
  setMotorSpeed(0);
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

  if (motorID == 0x01) {
    Serial1.write(packet, 10);
  } else {
    Serial2.write(packet, 10);
    Serial.println("debug 0x02 write");
  }
}

void waitForUserInput() {
  while (Serial.available() <= 0) {}
}

void checkID(){
  Serial.print("Current ID: ");
  Serial.println(motorID);
}