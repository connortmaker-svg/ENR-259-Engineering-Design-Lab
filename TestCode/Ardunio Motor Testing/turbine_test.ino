#include <Servo.h>

Servo esc;

void setup() {
    Serial.begin(9600);
    // Attach to pin 9 with wide range (500-20000us)
    esc.attach(9, 500, 20000);
    
    // IMPORTANT: Send STOP signal immediately to arm (stop the beeping)
    esc.writeMicroseconds(1000); 
    
    Serial.println("=========================================");
    Serial.println("       ESC MOTOR CONTROL READY           ");
    Serial.println("=========================================");
}

void loop() {
    if (Serial.available() > 0) {
        String input = Serial.readStringUntil('\n');
        input.trim(); // remove whitespace
        
        if (input == "HIGH" || input == "2") {
            esc.writeMicroseconds(2000); // Max Throttle
            Serial.println("Sending MAXIMUM throttle (2000us)... Plug in battery NOW!");
        }
        else if (input == "LOW" || input == "1" || input == "ARM") {
            esc.writeMicroseconds(1000); // Min Throttle
            Serial.println("Sending MINIMUM (1000us). ESC should arm/reset.");
        }
        else {
            int val = input.toInt();
            // Safety: Don't allow extremely low values that differ from 0/stop might confuse ESC
            if (val >= 1000 && val <= 20000) {
                esc.writeMicroseconds(val);
                Serial.print("Speed set to: ");
                Serial.println(val);
            }
            else if (val > 0 && val < 1000) {
                 Serial.println("Value too low! Sending 1000 (Stop) to be safe.");
                 esc.writeMicroseconds(1000);
            }
            else {
                 Serial.println("Ignored invalid value. Type '1' to ARM/STOP.");
            }
        }
    }
}