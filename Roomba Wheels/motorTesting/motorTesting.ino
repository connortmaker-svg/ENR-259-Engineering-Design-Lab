#include "Arduino.h";

void setup() {
  Serial1.begin(115200);
  Serial.begin(115200);
  
/* CHANGE ID:
  //make sure the argument is hex!
  int ID = 0x01;
  changeID(ID);
 
  uint8_t* response = {0};
  response = getID();
  char buffer[100];
  buffToHex(buffer, response);
  Serial.println("");
  Serial.println("--data--");
  Serial.println(buffer);
*/

  // speed check
  char buff2[100];
  int speed = 500;
  speedToHex(speed, buff2);
  Serial.println("");
  Serial.println("--data--");
  Serial.println(buff2);
}




void loop() {
  
}

void changeID(uint8_t motorID){
  byte id[10] = { 0xAA, 0x55, 0x53, motorID, 0x00,0x00,0x00,0x00,0x00,0x00};
  delay(100);
  Serial1.write(id,10);
}

void breakMotor(){
  byte id[10] = { 0x01, 0x64, 0x00, 0x00, 0x00,0x00,0x00,0xFF,0x00,0xD1};
  delay(100);
  Serial1.write(id,10);
}

uint8_t* getID() {
  byte id[10] = { 0xC8, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xDE };
  delay(100);
  Serial1.write(id, 10);

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
    if (Serial1.available()) {
      buffer[index++] = Serial1.read();
    }
  }
  return buffer;
}

//from -2100 to 2100 
int8_t* speedToHex(int speed, int8_t* buffer){
  if(speed < 0){
    speed = 0xFFFF - speed + 1;
  } 
  Serial.println("speed:");
  Serial.println(speed);
  sprintf(buffer, "%04X", speed);
  
  return buffer;
}





