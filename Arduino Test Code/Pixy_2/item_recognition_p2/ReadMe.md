# Pixy2 Programs
## Pixy2CCC
Attached in this folder is Pixy2CCC. I created a few functions that will likely be helpful for debugging. 

### New Functions:

Retrievers:

area() & signature()

DataDisplay:

printInfo()

## Item Recognition With Servo
This program uses the pixy camera to identify the color of a ball as it passes and communicate this color to the servo so it moves to the proper position
- Uses the SPI_SS communication mode (Uses SPI_SS specific library for pixy functions)
- Filters false positives by having a minumum area for color detection ( Area > 4000)
- Additional camera settings can be adjusted in the PixyMon program
- Still need to consider the timing between detection and servo actuation

## Interrupt Item Recognition
This program uses interrputs to sample the pixy camera so it can identify the color of a ball as it passes and communicates this color to the sorting servo
- Functionaly the same as the item recognition with servo program
- Compatable only with the Teensy 4.1 board
- Requires TimerOne.h header file
  
