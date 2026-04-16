#include "Adafruit_VL53L0X.h"

#define PERIOD 100
#define MAX_DISTANCE 100

unsigned long time_now = 0;

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

void setup() {
  Serial.begin(115200);

  // wait until serial port opens for native USB devices
  while (!Serial) {
    delay(1);
  }

  // Serial.println("Adafruit VL53L0X test");
  if (!lox.begin()) {
    //  Serial.println(F("Failed to boot VL53L0X"));
    while (1)
      ;
  }
  // power
  // Serial.println(F("VL53L0X API Simple Ranging example\n\n"));
}


void loop() {
  time_now = millis();
  VL53L0X_RangingMeasurementData_t measure;
  // Serial.print("Reading a measurement... ");
  lox.rangingTest(&measure, false);  // pass in 'true' to get debug data printout!


  while (millis() < time_now + PERIOD) {
    if (measure.RangeStatus != 4) {  // phase failures have incorrect data
      if (measure.RangeMilliMeter < MAX_DISTANCE) {
        Serial.print("in range.");
      }
    } else {
      // Serial.println(" out of range ");
    }
  }
}
