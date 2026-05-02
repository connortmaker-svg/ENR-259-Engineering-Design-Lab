#include <Servo.h>

Servo ms1;  
Servo ms2;
Servo ms3;

void setup() {
  ms1.attach(37); 
  ms2.attach(36);  
  ms3.attach(33);  

}

void loop() {
    ms1.write(90);              
    delay(15);   
    ms2.write(90);
    delay(15); 
    ms3.write(90);

    delay(3000);  

    ms1.write(180);              
    delay(15);   
    ms2.write(180);
    delay(15); 
    ms3.write(180);   

    delay(3000);               
}