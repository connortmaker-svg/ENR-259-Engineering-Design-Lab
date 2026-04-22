/*
  This program is for testing the intake motor with PWM control. (Don't use without proper bridge)
  It will ask for a duty cycle percentage (0-100%) and set the motor to the entered DC
  The current DC will be displayed once the motor is running and it will ask for the next input
*/


#include "Teensy_PWM.h"

//Create PWM instance
Teensy_PWM* PWM_Instance;

#define BAUD_RATE = 115200  
#define PWM_FREQ = 47400.0f //Default freq for the Teensy 4.1
#define PWM_PIN = 4         //Can be changed


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  //Initalize a new PWM instance
  PWM_Instance = new Teensy_PWM(PWM_PIN, 500.0f, 0.0f);
  
}

void loop() {
  // put your main code here, to run repeatedly:
    //Query user and wait for desired DC
    Serial.println("Enter Duty Cycle (0-100%):");
    while(Serial.available() < 1) {}

    //Read data from buffer
    int duty_cycle_int = Serial.parseInt();   //Parse int causes the data to be read as an integer
    float duty_cycle_f = duty_cycle_int;      //Convert to float
    
    
    //Sets the motor speed using the configured PWM DC
    PWM_Instance->setPWM(PWM_PIN, 1.0f, duty_cycle_f);
    

    //Display Entered speed   
    Serial.print("DC set to : ");
    Serial.print(duty_cycle_int);
    Serial.println("%");

    
    //This block is used to clear the serial buffer 
    while (Serial.available() > 0) {
      Serial.read();
    }
    
  

}
