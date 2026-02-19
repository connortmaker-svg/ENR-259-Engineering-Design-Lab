#include "Arduino.h";

// to do:
// see if you can switch hex representation to decimal for ease of use
// 500 << left and right to get MSB And LSB for hex for speed
// CRC8 arduino calculate
// put them all together to create a function that you can set the speed to anything

byte idd[10] = {0x01, 0x64, 0xFE, 0x0C, 0x00,0x00,0x00,0x00,0x00,0x16};


void setup() {


  Serial1.begin(115200);
  Serial.begin(115200);
 
  byte id[10] = { 0xAA, 0x55, 0x53, 0x01, 0x00,0x00,0x00,0x00,0x00,0x00};
  
  Serial1.write(id,10);
  delay(100);
  Serial1.write(id,10);
  delay(100);
  Serial1.write(id,10);
  delay(100);
  Serial1.write(id,10);
  delay(100);
  Serial1.write(id,10);
  delay(100);
  Serial1.write(id,10);
  delay(100);

  byte idd[10] = {0x01, 0x64, 0x07, 0xD0, 0x00,0x00,0x00,0x00,0x00,0x27};
  delay(100);
  Serial1.write(idd,10);

}

void loop() {
  Serial1.write(idd,10);
  delay(500);
  
  if (Serial1.available() > 0) {
    // read the incoming byte:
    int incomingByte = Serial1.read();
 
    // say what you got:
    Serial.print("I received: ");
    Serial.println(incomingByte, DEC);
  }
}

void changeID(uint16_t motorID){
  byte id[10] = { 0xAA, 0x55, 0x53, motorID, 0x00,0x00,0x00,0x00,0x00,0x00};
  delay(100);
  
  for (int i = 0; i<5; i++){
    Serial.write(id,10);
    delay(500);
  }
}

void breakMotor(){
  byte id[10] = { 0x01, 0x64, 0x00, 0x00, 0x00,0x00,0x00,0xFF,0x00,0xD1};
  delay(100);
  Serial.write(id,10);
}


uint16_t* getID() {
  byte id[10] = { 0xC8, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xDE };
  delay(100);
  Serial.write(id, 10);
  uint16_t response[10];  // Fixed: array of uint16_t, not array of pointers
  readResponse(response, 10);  // Pass array (decays to pointer)
  return response; 
}


// for debug
uint8_t* buffToHex(uint8_t* buffer, uint8_t* data){
  sprintf(buffer, "%02X %02X %02X %02X %02X %02X %02X %02X %02X %02X", 
  data[0],data[1],data[2],data[3],data[4],
  data[5],data[6],data[7],data[8],data[9]);
  return buffer;
}

// from BricoGeek_DirectDriveServo.cpp
uint16_t* readResponse(uint16_t* buffer, uint16_t length) {
  uint16_t index = 0;
  unsigned long startTime = millis();
  while (index < length && (millis() - startTime) < 1000) {
    if (Serial3.available()) {
      buffer[index++] = Serial3.read();
    }
  }
  return buffer;
}

void go50(uint16_t motorID){
  byte id[10] = {motorID, 0x64, 0xFE, 0x0C, 0x00,0x00,0x00,0x00,0x00,0x16};
  delay(100);
  Serial.write(id,10);
}

void setModeLoop(uint16_t motorID){
  byte id[10] = {motorID, 0xA0, 0x02, 0x00, 0x00,0x00,0x00,0x00,0x00,0xE4};
  delay(100);
  Serial.write(id,10);
}

int16_t* decimalToHex(int16_t num){
      int16_t high = (num >> 8) & 0xFF;  
      int16_t low = num & 0xFF;
      int16_t hex[2] = {high, low};
      return hex;
}

//from -2100 to 2100
void goSpeed(uint16_t motorID, uint16_t speed){

  int16_t* speedHex = {};
  speedHex = decimalToHex(speed);
  int16_t high = speedHex[0];
  int16_t low = speedHex[1];
  byte command[10] = {motorID, 0x64, high, low, 0x00,0x00,0x00,0x00,0x00,0x16};
  Serial.write(command,10);
}



