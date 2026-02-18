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
  
  delay(100);
  Serial1.write(idd,10);
  delay(100);

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

void changeID(uint8_t motorID){
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

uint8_t* getID() {
  byte id[10] = { 0xC8, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xDE };
  delay(100);
  Serial.write(id, 10);

  uint8_t response[10];  // Fixed: array of uint8_t, not array of pointers
  readResponse(response, 10);  // Pass array (decays to pointer)
  

  return response; 
}



uint8_t* buffToHex(uint8_t* buffer, uint8_t* data){
  sprintf(buffer, "%02X %02X %02X %02X %02X %02X %02X %02X %02X %02X", 
  data[0],data[1],data[2],data[3],data[4],
  data[5],data[6],data[7],data[8],data[9]);
  return buffer;
}


uint8_t* readResponse(uint8_t* buffer, uint8_t length) {
  uint8_t index = 0;
  unsigned long startTime = millis();
  while (index < length && (millis() - startTime) < 1000) {
    if (Serial3.available()) {
      buffer[index++] = Serial3.read();
    }
  }
  return buffer;
}

//from -2100 to 2100 
int8_t* speedToHex(int speed, int8_t* buffer){
  if(speed < 0){
    speed = 0xFFFF - speed + 1;
  } 
 
  sprintf(buffer, "%04X", speed); //format for buffer
  return buffer;
}


void go50(uint8_t motorID){
  byte id[10] = {motorID, 0x64, 0xFE, 0x0C, 0x00,0x00,0x00,0x00,0x00,0x16};
  delay(100);
  Serial.write(id,10);
}

void setModeLoop(uint8_t motorID){
  byte id[10] = {motorID, 0xA0, 0x02, 0x00, 0x00,0x00,0x00,0x00,0x00,0xE4};
  delay(100);
  Serial.write(id,10);
}






