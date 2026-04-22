## Motor Test
This program is for testing the intake motor with PWM control via the Teensy 4.1
- Uses the Teensy_PWM library to generate PWM signals (Hardware)
- Output is set to pin 4 of the Teensy
- Uses the serial terminal to ask for the desired duty cycle

### Notes
- The Teensy_PWM libary utilizes floating point numbers for the duty cycle and frequency
- Since the PWM is in hardware it will not be effected by halting functions
- The serial buffer needs to be cleared after receiving data so it is not read again
