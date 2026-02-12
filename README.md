# ENR-259-Engineering-Design-Lab

## Schedule Visualizer
The `ScheduleVisualizer` folder contains a Python program designed to help teams facilitate finding common meeting times. It allows users to input their availability and provides a visual heatmap of the team's schedule. This tool was created briefly to assist with quick coordination.

## Arduino Motor Testing
The `Ardunio Motor Testing` folder contains code for testing and controlling an ESC (Electronic Speed Controller) and motor using an Arduino.

### Usage
- **Upload**: Upload `turbine_test.ino` to your Arduino.
- **Serial Monitor**: Open the Serial Monitor at **9600 baud**.
- **Commands**:
  - `ARM` / `1` / `LOW`: Sends 1000us pulse (minimum throttle) to arm the ESC. **Do this first to stop the beeping.**
  - `HIGH` / `2`: Sends 2000us pulse (maximum throttle). Use with caution.
  - `[1000-20000]`: Enter a specific microsecond value to set the speed.

## Pixy 2 Item Recognition
The `Pixy_2` folder contains code for object detection and color signature recognition using the Pixy2 camera.

### Features
- Uses a modified `Pixy2CCC` library for enhanced item recognition.
- Filters detected blocks based on area threshold (>40000).
- Prints specific info about the detected item based on its signature.

### Setup
- **Hardware**: Ensure the Pixy2 camera is connected to the Arduino (ISCP headers).
- **Positioning**: The object should be placed approximately **3.5 cm** away from the Pixy2 camera (measured from the diameter of the object to the board itself).
- **Lighting**: The code sets the lamp to maximum brightness for optimal detection.
