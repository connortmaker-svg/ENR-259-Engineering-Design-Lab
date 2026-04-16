/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 https://www.arduino.cc/en/Tutorial/LibraryExamples/Sweep
*/

#include <Servo.h>

Servo myservo;  // create Servo object to control a servo
// twelve Servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

void setup() {
  myservo.attach(5);  // attaches the servo on pin 9 to the Servo object
}

void loop() {
    Serial.println("Please give servo val: ");
    waitForUserInput();  
    sel();
    
}

void sel(){
  if (Serial.available() > 0) {
    int sel = Serial.parseInt();
    //clear buffer
    while (Serial.available()) {
      Serial.read();
    }
    myservo.write(sel);
    delay(100);
  }
}

void waitForUserInput() {
  while (Serial.available() <= 0) {}
}


