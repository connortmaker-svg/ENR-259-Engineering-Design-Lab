#include <Servo.h>

Servo redServo;
Servo blueServo;
Servo whiteServo;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.print("Starting...\n");

  redServo.attach(4);
  redServo.write(180);

  blueServo.attach(5);
  blueServo.write(180);

  whiteServo.attach(6);
  whiteServo.write(180);

}

void loop() {
  // put your main code here, to run repeatedly:
  delay(2000);
  redServo.write(90);
  delay(2000);
  blueServo.write(90);
  delay(2000);
  whiteServo.write(90);
  delay(2000);
  Serial.print("All balls dropped\n");
  redServo.write(180);
  blueServo.write(180);
  whiteServo.write(180);
}
