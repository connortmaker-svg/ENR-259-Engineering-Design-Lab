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

## Roomba Wheels
The `Roomba Wheels` folder contains code for controlling Roomba wheels using a custom driver setup (based on the `M0603B` or similar driver logic).

### Contents
- **libFramework**: Contains the core driver library (`M0603B`).
- **motorTesting**: A sketch for testing the motor movement and validating the driver logic.

### Usage
- Ensure your hardware connections match the pin definitions in the library.
- Upload the `motorTesting.ino` sketch to verify wheel rotation and direction control.

## Sensor Test Code
The `SensorTestCode` folder includes various sketches and scripts for testing sensors and logging data.

### Contents
- **vl53l0-test**: Arduino sketch for the VL53L0X Time-of-Flight distance sensor.
- **Python Scripts**:
  - `vl53l0-csv.py`: Reads serial data from the VL53L0X sensor and saves it to a CSV file.
  - `serial_monitor_to_csv.py`: A generic script to capture serial monitor output to a CSV file.

### Usage
- **VL53L0X**: Connect the sensor via I2C, upload `vl53l0-test.ino`, and use the Python script to log distance measurements.
- **General Logging**: Use `serial_monitor_to_csv.py` to capture data from any Arduino sketch outputting to the serial monitor.
