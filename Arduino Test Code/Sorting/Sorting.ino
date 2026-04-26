#include <Servo.h>

#include "Pixy2SPI_SS.h"

Pixy2SPI_SS pixy;
Servo ms_red;
Servo ms_white;
Servo ms_blue;
Servo seat;

int input1 = 14;
int input2 = 15;
int enable = 36;

void setup() {
  //update with proper servo pins
  ms_red.attach(1);
  ms_white.attach(2);
  ms_blue.attach(3);
  //servo seat
  seat.attach(4);

  Serial.begin(115200);
  Serial.print("Starting...\n");

  pixy.init();
  pixy.setLamp(255, 255);

  //initialize IC
  pinMode(enable, OUTPUT);
  pinMode(input1, OUTPUT);
  pinMode(input2, OUTPUT);
  digitalWrite(enable, HIGH);
  digitalWrite(input1, LOW);

  analogWrite(input2, 220); //turn on intake
}

void loop() {
  detectBalls();
}

void detectBalls() {
  pixy.ccc.getBlocks();

  if (pixy.ccc.numBlocks > 0) {
    //Find area and age of block
    uint16_t area = pixy.ccc.blocks[0].area();

    // if the area of the block is above a certain threshold, read the signature
    if (area > 4000) {
      uint16_t signature = pixy.ccc.blocks[0].m_signature;
      switch (signature) {

        //Red ball detected
      case 1:
        Serial.println("Red ball detected");
        pixy.ccc.blocks[0].printInfo();
        seat.write(50);
        break;

        //White ball detected
      case 2:
        Serial.println("White ball detected");
        pixy.ccc.blocks[0].printInfo();
        seat.write(100);
        break;
        //Blue ball detected
      case 3:
        Serial.println("Blue ball detected");
        pixy.ccc.blocks[0].printInfo();
        seat.write(150);
        break;
      default:

        break;
      }
    }

  }
}