/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 https://www.arduino.cc/en/Tutorial/LibraryExamples/Sweep
*/

#include <Servo.h>
#include <Arduino.h>

Servo myservo;  // create Servo object to control a servo
// twelve Servo objects can be created on most boards


void setup() {
  Serial.begin(9600);
  pinMode(A2, INPUT);
  myservo.attach(3);  // attaches the servo on pin 9 to the Servo object
  myservo.write(0); //set to og pos
}

void loop() {
    Serial.println();
    int batlvl = analogRead(A2);
    while(batlvl < 500){
    myservo.write(0);
    delay(3000);
    myservo.write(180); 
    delay(2000);
    batlvl = analogRead(A2);
    }

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


