//demo main
//demo code to "run" each component individually
#include "TurbineControl.h"

#include <Servo.h>

#include "Pixy2SPI_SS.h"


// Initialize turbine on pin 9
TurbineControl turbine(9);
Pixy2SPI_SS pixy;
Servo myservo;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); //might cause issues?
  Serial.print("Starting...\n");
  myservo.attach(9); //pin 9
  pixy.init();
  //set lamp to max for best item detection
  pixy.setLamp(255, 255);

  turbine.begin();
  // Arm the motor on startup
  turbine.arm();

  Serial.println("Turbine Lib ready");
  Serial.println("Pixy Ready");
  Serial.println("Servo Ready");
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

// turn on turn off fan
// color sense one ball and move the servo accordingly
// 



int lookForBalls() {
  int flag = 0;
  // grab blocks!
  pixy.ccc.getBlocks();

  // If there are detected blocks, print them
  if (pixy.ccc.numBlocks > 0) {
    //Find area and age of block
    uint16_t area = pixy.ccc.blocks[0].area();

    // if the area of the block is above a certain threshold, read the signature
    if (area > 4000) {
      // first item in pixy.ccc.blocks (always has the most area out of the blocks found)
      // printInfo is a modification to the pixy2 library. It checks the signature (1-3.
      // each of which is pre-defined as a certain golf ball color) and then prints which
      // signature it is.
      //pixy.ccc.blocks[0].printInfo();
      uint16_t signature = pixy.ccc.blocks[0].m_signature;
      //Switch statment with a case for each ball color (1-red, 2-white, 3-blue)
      switch (signature) {
        //Red ball detected
      case 1:
        Serial.println("Red ball detected");
        pixy.ccc.blocks[0].printInfo();
        myservo.write(50);
        flag = 1;
        break;
        //White ball detected
      case 2:
        Serial.println("White ball detected");
        pixy.ccc.blocks[0].printInfo();
        myservo.write(100);
        flag = 1;
        break;
        //Blue ball detected
      case 3:
        Serial.println("Blue ball detected");
        pixy.ccc.blocks[0].printInfo();
        myservo.write(150);
        flag = 1;
        break;
      default:

        break;
      }
      return flag;
    }

  }
  return flag;
}

void debugPrintout() {
  Serial.println("  \n  -------- MENU --------");
  Serial.println("1.             Set Fan Speed");
  Serial.println("2.            Check and Turn");
}

void debugMenu(int selection) {
  delay(200);
  switch (selection) {
  case 1:
    String speed = "";
    Serial.println("Please input speed: ");
    while (Serial.available() <= 0) {}
    if (Serial.available() > 0) {
    speed = Serial.readStringUntil('\n');
    }
    turbine.debugControl_inp(speed);
    break;
  case 2:
    lookForBalls();
    break;
  }
}

void waitForUserInput() {
  while (Serial.available() <= 0) {}
}