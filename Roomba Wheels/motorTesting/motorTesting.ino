#define MOTOR_ID_LEFT 0x01
#define MOTOR_ID_RIGHT 0x02
#define BAUD_RATE 115200


// to do:
1.) Define which wheel is left and which is right
2.) add ID field to set motor speed
3.) create ID setting function
5.) create angle function
6.) Set mode (RPM or angle) function
7.) turning function turn left turn right by x degrees etc.

void setup() {
  Serial.begin(115200); // Debug Monitor
  Serial1.begin(BAUD_RATE); // Motor Port
  setMotorSpeed(20);

  delay(100); // Wait before next loop
  debugPrintout();
}

void loop() {
  if (Serial.available() > 0) {
    int sel = Serial.parseInt();
    //clear buffer
    while(Serial.available()) { Serial.read(); }
    debugMenu(sel);
    debugPrintout();
  }
}

void debugPrintout(){
  Serial.println("\n-------- MENU --------");
  Serial.println("1. Set Motor Speed Left");
  Serial.println("2. Set Motor Speed Right");
  Serial.println("   Input Choice: ");
}

void debugMenu(int selection) {
  switch (selection) {
    case 1:
      break;
    case 2: {
      Serial.print("Enter Speed (-210 to 210): ");

      // 
      waitForUserInput();

      // Read speed
      int speed = Serial.parseInt();

      //clear buffer
      while(Serial.available()) { Serial.read(); }

      Serial.print("Setting speed to: ");
      Serial.println(speed);
      delay(10);
      setMotorSpeed(speed);

    } break;

    default:
      Serial.println("Invalid choice.");
    break;
  }
}

// send speed command
void setMotorSpeed(int16_t rpm) {
  int16_t speedValue = rpm * 10;
  uint8_t packet[10] = {
    MOTOR_ID, 0x64, 
    (speedValue >> 8) & 0xFF, speedValue & 0xFF, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };
  calculateAndSend(packet);
}


void requestMotorStatus() {
  
  uint8_t packet[10] = {
    MOTOR_ID, 0x74, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };

  calculateAndSend(packet);
}

// calc crc and send to motor
void calculateAndSend(uint8_t *packet) {
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
  Serial1.write(packet, 10);
}


void waitForUserInput() {
  while (Serial.available() <= 0) {}
}
